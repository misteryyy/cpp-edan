/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolhandler.h"
#include "protocol.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace protocol;
using namespace client_server;

using client_server::Connection;
using client_server::ConnectionClosedException;

static const int     LIST_NG    = 1;
static const int     CREATE_NG  = 4;
static const int     DELETE_NG  = 5;
static const int     LIST_ART   = 2;
static const int     CREATE_ART = 6;
static const int     DELETE_ART = 7;
static const int     READ_ART   = 3;
ProtocolHandler *ph;

//if the next command is i return true else write error and return false
bool expectCommand(int i){
	int command=ph->readChar();
	if(command!=i){
//		cerr<<"Server error, client did not receive command"<<i<<" command was: "<<command<<endl;
		return false;
	}else{
		return true;
	}
}
//COM_LIST_NG COM_END
//ANS_LIST_NG num_p [num_p string_p]* ANS_END
void listNg(){
	ph->writeChar(Protocol::COM_LIST_NG);
	ph->writeChar(Protocol::COM_END);
	if(expectCommand(Protocol::ANS_LIST_NG)){
		int nbrOfNg=ph->readProtocolNbr();
		cout<<"<--Newsgroups-->"<<endl;
		for(int i=0;i<nbrOfNg;i++){
			int ng_num=ph->readProtocolNbr();
			string title=ph->readProtocolString();
			cout<<ng_num<<": "<<title<<endl;
		}

	}else{
		cerr<<"Failed to list newsgroups"<<endl;
	}
	expectCommand(Protocol::ANS_END);
}
//COM_CREATE_NG string_p COM_END
//ANS_CREATE_NG [ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS] ANS_END
void createNg(string& title){
		ph->writeChar(Protocol::COM_CREATE_NG);
		ph->writeProtocolString(title);
		ph->writeChar(Protocol::COM_END);
		expectCommand(Protocol::ANS_CREATE_NG);
		if(expectCommand(Protocol::ANS_ACK)){
			cout<<"Newsgroup: "<<title<<" was successfully created!"<<endl;
		}else if(expectCommand(Protocol::ANS_NAK)){
			ph->readChar();// read ERR_NG_ALREADY_EXISTS
			cerr<<"Error newsgroup with title: "<<title<<" already exists "<<endl;
		}else{
			cerr<<"Server error, unknown command"<<endl;
		}
		expectCommand(Protocol::ANS_END);
}
//COM_DELETE_NG num_p COM_END
//ANS_DELETE_NG [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void deleteNg(int ng_nbr){
		ph->writeChar(Protocol::COM_DELETE_NG);
		ph->writeProtocolNbr(ng_nbr);
		ph->writeChar(Protocol::COM_END);
		if(expectCommand(Protocol::ANS_DELETE_NG)){
			if(expectCommand(Protocol::ANS_ACK)){
				cout<<"Newsgroup "<<ng_nbr<<" successfully deleted"<<endl;
			}else if(expectCommand(Protocol::ANS_NAK)){
				expectCommand(Protocol::ERR_ART_DOES_NOT_EXIST);
				cerr<<"Failed to delete newsgroup "<<ng_nbr<<", it does not exist"<<endl;
			}else{
				cerr<<"Server error, unknown command"<<endl;
			}
		}
		expectCommand(Protocol::ANS_END);
}
//COM_LIST_ART num_p COM_END
//ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* |ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void listArt(int ng_num){
		ph->writeChar(Protocol::COM_LIST_ART);
		ph->writeProtocolNbr(ng_num);
		ph->writeChar(Protocol::COM_END);
		if(expectCommand(Protocol::ANS_LIST_ART)){
			if(expectCommand(Protocol::ANS_ACK)){
				int nbrOfArt=ph->readProtocolNbr();
				cout<<"<--Articles in NG: "<<ng_num<<"-->"<<endl;
				for(int i=0;i<nbrOfArt;i++){
					int art_num=ph->readProtocolNbr();
					string art_name=ph->readProtocolString();
					cout<<art_num<<": "<<art_name<<endl;
				}
			}else if(expectCommand(Protocol::ANS_NAK)){
				expectCommand(Protocol::ERR_NG_DOES_NOT_EXIST);
				cerr<<"Failed to list articles because NG "<<ng_num<<" does not exist"<<endl;
			}
		}else{
			cerr<<"Server error when listing articles, unknown command"<<endl;
		}
		expectCommand(Protocol::ANS_END);
}
//COM_CREATE_ART num_p string_p string_p string_p COM_END
//ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void createArt(int ng_num, string title, string author, string text){
		ph->writeChar(Protocol::COM_CREATE_ART);
		ph->writeProtocolNbr(ng_num);
		ph->writeProtocolString(title);
		ph->writeProtocolString(author);
		ph->writeProtocolString(text);
		ph->writeChar(Protocol::COM_END);
		if(expectCommand(Protocol::ANS_CREATE_ART)){
			if(expectCommand(Protocol::ANS_ACK)){
				cout<<"Article: "<<title<<" successfully created!"<<endl;
			}else if(expectCommand(Protocol::ANS_NAK)){
				expectCommand(Protocol::ERR_NG_DOES_NOT_EXIST);
				cerr<<"Failed to create article: "<<title<<" in NG: "<<ng_num<<", NG does not exist"<<endl;
			}else{
				cerr<<"Server error when creating article, unknown command"<<endl;
			}
		}else{
			cerr<<"Server error when creating article, unknown command"<<endl;
		}
		expectCommand(Protocol::ANS_END);
}
//group and article (num_p)
//COM_DELETE_ART num_p num_p COM_END
//ANS_DELETE_ART [ANS_ACK |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void deleteArt(int ng_nbr, int art_nbr){
		ph->writeChar(Protocol::COM_DELETE_ART);
		ph->writeProtocolNbr(ng_nbr);
		ph->writeProtocolNbr(art_nbr);
		ph->writeChar(Protocol::COM_END);
		if(expectCommand(Protocol::ANS_DELETE_ART)){
			if(expectCommand(Protocol::ANS_ACK)){
				cout<<" Article with id: "<<art_nbr<<" in NG: "<<ng_nbr<<" was successfully deleted"<<endl;
			}else if(expectCommand(Protocol::ANS_NAK)){
				if(expectCommand(Protocol::ERR_NG_DOES_NOT_EXIST)){
					cerr<<"Error when deleting article, newsgroup does not exist"<<endl;
				}else if(expectCommand(Protocol::ERR_ART_DOES_NOT_EXIST)){
					cerr<<"Error when deleting article, article does not exist"<<endl;
				}
			}else{
				cerr<<"Server error when removing article, unknown command"<<endl;
			}
		}else{
			cerr<<"Server error when removing article, unknown command"<<endl;
		}
		expectCommand(Protocol::ANS_END);
}
//COM_GET_ART num_p num_p COM_END
//ANS_GET_ART [ANS_ACK string_p string_p string_p |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void readArt(int ng_nbr,int art_nbr){
		ph->writeChar(Protocol::COM_GET_ART);
		ph->writeProtocolNbr(ng_nbr);
		ph->writeProtocolNbr(art_nbr);
		ph->writeChar(Protocol::COM_END);
		if(expectCommand(Protocol::ANS_GET_ART)){
			if(expectCommand(Protocol::ANS_ACK)){
				string title=ph->readProtocolString();
				string author=ph->readProtocolString();
				string text=ph->readProtocolString();
				cout<<"------------"<<endl;
				cout<<title<< " by "<<author<<endl;
				cout<<endl;
				cout<<text<<endl;
				cout<<"------------"<<endl;
			}else if(expectCommand(Protocol::ANS_NAK)){
				if(expectCommand(Protocol::ERR_NG_DOES_NOT_EXIST)){
					cerr<<"Error when reading article, newsgroup does not exist"<<endl;
				}else if(expectCommand(Protocol::ERR_ART_DOES_NOT_EXIST)){
					cerr<<"Error when reading article, article does not exist"<<endl;
				}
			}
		}else{
			cerr<<"Server error when reading article, unknown command"<<endl;
		}
		expectCommand(Protocol::ANS_END);
}
void printMenu(){
	cout<<"------------"<<endl;
	cout<<"Please enter the number for one of the following commands to be executed:"<<endl;
	cout<<"1:ListNg"<<endl;
	cout<<"2:ListArticles"<<endl;
	cout<<"3:ReadArticle"<<endl;
	cout<<"4:AddNg"<<endl;
	cout<<"5:DeleteNg"<<endl;
	cout<<"6:AddArticle"<<endl;
	cout<<"7:DeleteArticle"<<endl;
	cout<<"------------"<<endl;
}
void processCommand(string command){
	int choice;
	stringstream css(command);
	css>>choice;
	if((LIST_NG)==choice){
		listNg();
	}else if((CREATE_NG)==choice){
		string title;
		cout<<"Please enter title"<<endl;
		getline(cin, title);
		createNg(title);
	}else if((DELETE_NG)==choice){
		string ng_num;
		cout<<"Enter the id for the newsgroup to be deleted"<<endl;
		getline(cin, ng_num);
		stringstream ss(ng_num);
		int i;
		ss>>i;
		deleteNg(i);
	}else if((LIST_ART)==choice){
		string ng_num;
		cout<<"Enter the id for the newsgroup to list articles for"<<endl;
		getline(cin, ng_num);
		stringstream ss(ng_num);
		int i;
		ss>>i;
		listArt(i);
	}else if((CREATE_ART)==choice){
		cout<<"Enter the id for the newsgroup to create a article in"<<endl;
		string tmp;
		getline(cin, tmp);
		stringstream ss(tmp);
		int ng_num;
		ss>>ng_num;
		cout<<"Enter the title of the article:"<<endl;
		string title;
		getline(cin, title);
		cout<<"Enter the author of the article:"<<endl;
		string author;
		getline(cin, author);
		cout<<"Enter the article text:"<<endl;
		string text;
		getline(cin, text);
		createArt(ng_num,title,author,text);
	}else if((DELETE_ART)==choice){
		string tmp;
		cout<<"Enter the newsgroup id (delete art)"<<endl;
		getline(cin, tmp);
		stringstream ss(tmp);
		int ng_nbr;
		ss>>ng_nbr;
		cout<<"Enter the article id (delete art)"<<endl;
		tmp="";
		getline(cin, tmp);
		stringstream ss2(tmp);
		int art_nbr;
		ss2>>art_nbr;
		deleteArt(ng_nbr,art_nbr);
	}else if((READ_ART)==choice){
		string tmp;
		cout<<"Enter the newsgroup id (read art)"<<endl;
		getline(cin, tmp);
		stringstream ss(tmp);
		int ng_nbr;
		ss>>ng_nbr;
		cout<<"Enter the article id (read art)"<<endl;
		tmp="";
		getline(cin, tmp);
		stringstream ss2(tmp);
		int art_nbr;
		ss2>>art_nbr;
		readArt(ng_nbr,art_nbr);
	}else{
		cout<<"Bad command: "<<choice<<endl;
	}
}
int main(int argc, char* argv[]) {
	string hostname="localhost";
	int port=30004;
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		cout<<"Using host name->localhost port number->"<<port<<" instead"<<endl;
	}else{// arguments found
		hostname=argv[1];
		port=atoi(argv[2]);
	}
	Connection conn(hostname.c_str(), port);

	ph=new ProtocolHandler(conn);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	cout << "Connection successfully established!"<<endl;
	string command;
	printMenu();
	while (getline(cin, command)) {
		try {
//			cout<<command<<endl;
			processCommand(command);
		} catch (ConnectionClosedException&) {
			cerr << "Server closed down!" << endl;
			exit(1);
		}
		printMenu();
	}
}
