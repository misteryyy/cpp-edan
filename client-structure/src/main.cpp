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

static const string 	LIST_NG    = "ListNg";
static const string     CREATE_NG  = "AddNg";
static const string     DELETE_NG  = "DeleteNg";
static const string     LIST_ART   = "ListArticles";
static const string     CREATE_ART = "AddArticle";
static const string     DELETE_ART = "DeleteArticle";
static const string     READ_ART   = "ReadArticle";
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
			cout<<ph->readProtocolNbr()<<": "<<ph->readProtocolString()<<endl;
		}

	}else{
		cerr<<"Failed to list newsgroups"<<endl;
	}
	expectCommand(Protocol::ANS_END);
}
//COM_CREATE_NG string_p COM_END
//ANS_CREATE_NG [ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS] ANS_END
void createNg(stringstream& ss){
	if(!ss.eof()){
		string title;
		ss>>title;
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

//		int command=ph->readChar();
//		if(command!=Protocol::ANS_CREATE_NG){
//			cerr<<"Server error, client did not receive ans_create_ng command, command was: "<<command<<endl;
//			return;
//		}
//		command=ph->readChar();
//		if(command==Protocol::ANS_ACK){
//			cout<<"Newsgroup: "<<title<<" was successfully created!"<<endl;
//		}else if(command==Protocol::ANS_NAK){
//			ph->readChar();// read ERR_NG_ALREADY_EXISTS
//			cerr<<"Error newsgroup with title: "<<title<<" already exists "<<command<<endl;
//		}else{
//			cerr<<"Server error, unknown command"<<endl;
//		}
//		command=ph->readChar();
//		if(command!=Protocol::ANS_END){
//			cerr<<"Server error, missing command ANS_END"<<endl;
//		}
	}else{
		cerr<<"Missing argument <title> for command AddNg <title>"<<endl;
	}
}
//COM_DELETE_NG num_p COM_END
//ANS_DELETE_NG [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void deleteNg(stringstream& ss){

}
//COM_LIST_ART num_p COM_END
//ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* |ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void listArt(stringstream& ss){

}
//COM_CREATE_ART num_p string_p string_p string_p COM_END
//ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void createArt(stringstream& ss){

}
//COM_DELETE_ART num_p num_p COM_END
//ANS_DELETE_ART [ANS_ACK |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void deleteArt(stringstream& ss){

}
//COM_GET_ART num_p num_p COM_END
//ANS_GET_ART [ANS_ACK string_p string_p string_p |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void readArt(stringstream& ss){

}
void printMenu(){
	cout<<"------------"<<endl;
	cout<<"Please enter one of the following commands to be executed:"<<endl;
	cout<<"ListNg"<<endl;
	cout<<"ListArticles <ng_id>"<<endl;
	cout<<"ReadArticle <ng_id> <art_id>"<<endl;
	cout<<"AddNg <title>"<<endl;
	cout<<"DeleteNg <ng_id>"<<endl;
	cout<<"AddArticle <ng_id> <title> <author> <text>"<<endl;
	cout<<"DeleteArticle <ng_id> <art_id>"<<endl;
	cout<<"------------"<<endl;
}
void processCommand(string command){
	stringstream ss(command);
	string firstcommand;
	ss>>firstcommand;
	if(firstcommand.compare(LIST_NG)==0){
		listNg();
	}else if(firstcommand.compare(CREATE_NG)==0){
		createNg(ss);
	}else if(firstcommand.compare(DELETE_NG)==0){
		deleteNg(ss);
	}else if(firstcommand.compare(LIST_ART)==0){
		listArt(ss);
	}else if(firstcommand.compare(CREATE_ART)==0){
		createArt(ss);
	}else if(firstcommand.compare(DELETE_ART)==0){
		deleteArt(ss);
	}else if(firstcommand.compare(READ_ART)==0){
		readArt(ss);
	}else{
		cout<<"Bad command: "<<firstcommand<<" be careful with upper/lower case"<<endl;
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
