/*
 * messagecontroller.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: buffalo
 */

#include "messagecontroller.h"
#include "core/protocol.h"
#include "core/connection.h"
#include "dao/storageinterface.h"
#include "dao/storagefactory.h"
#include "core/connectionclosedexception.h"
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace protocol;
using namespace std;
namespace client_server {

int readNumber(Connection * conn) throw (ConnectionClosedException) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
void writeNumber(int value, Connection& conn) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}
//string_p: PAR_STRING N char1 char2 ... charN // N is the number of characters
//num_p: PAR_NUM N
void writeProtocolString(Connection * conn, string const& mess) {
	conn->write(Protocol::PAR_STRING);
	int len = mess.size();
	writeNumber(len, *conn);
	for (int i = 0; i < len; i++) {
		conn->write(mess[i]);
	}
}
void writeProtocolNbr(Connection * conn, int val) {
	conn->write(Protocol::PAR_NUM);
	writeNumber(val, *conn);
}

int readProtocolNbr(Connection * conn){
	if(conn->read() == Protocol::PAR_NUM){
		cout << "INSIDE readProtocolNbr" << endl;
		int nbr = readNumber(conn);
		cout << "Nbr: " << nbr << endl;
		return nbr;
	}
	else{
		cerr << "Protocol failure: Bad command, expected PAR_NUM" << endl;
	}
		return 0;
		//Maybe should be system exit here!
}
string readProtocolString(Connection * conn){
	if(conn->read() == Protocol::PAR_STRING){
		int nbr = readNumber(conn);
		string mess;
		for(int i=0;i<nbr;i++){
			mess+=conn->read();
		}
		return mess;
	}
	else{
		cerr << "Protocol failure: Bad command, expected PAR_STRING" << endl;
	}
		return 0;
		//Maybe should be system exit here!
}

MessageController::MessageController() {
	storage = StorageFactory::getDAO("memory");
}

MessageController::~MessageController() {
	// TODO Auto-generated destructor stub
}

/*
void MessageController::writeString(const string& s, Connection * conn)
		throw (ConnectionClosedException) {
	for (size_t i = 0; i < s.size(); i) {
		conn->write(s[i]);
	}
	conn->write('$');
	// ‘$’ is end of string
}
*/

void MessageController::execute(int command, Connection * conn) {
	switch (command) {

	case Protocol::COM_LIST_NG:
		listNG(conn);
		break;
	case Protocol::COM_CREATE_NG:
		cout << "CREATE NG" << endl;
		createNG(conn);
		break;
	case Protocol::COM_DELETE_NG:
		deleteNG(conn);
		break;
	case Protocol::COM_LIST_ART:
		cout << "LIST ART" << endl;
		listArt(conn);
		break;
	case Protocol::COM_CREATE_ART:
		createArt(conn);
		break;
	case Protocol::COM_DELETE_ART:
		deleteArt(conn);
		break;
	case Protocol::COM_GET_ART:
		getArt(conn);
		break;

	default:
		break;

	}
}

string MessageController::readString(Connection* conn) {
	unsigned char type = conn->read();
	if (type != Protocol::PAR_STRING) {
		cerr << "fail: " << type << endl;
	}
	int lengthOfMessage = readNumber(conn);
//	unsigned char nbr;
	string mess;
	for (int i = 0; i < lengthOfMessage; i++) {
		mess += conn->read();
//		stringstream ss;
//		nbr = conn->read();
//		//cout << nbr << endl;
//		ss << mess << nbr;
//		ss >> mess;
	}
	return mess;
}

//COM_LIST_NG COM_END
//ANS_LIST_NG num_p [num_p string_p]* ANS_END
void MessageController::listNG(Connection * conn) {
	if(conn->read() != Protocol::COM_END){
		cerr << "Listing newsgroups failed" << endl;
	}
	else{
		conn->write(Protocol::ANS_LIST_NG);
		map<unsigned, Newsgroup> newsgroups = storage->listNg();
		writeProtocolNbr(conn, newsgroups.size());
		for (map<unsigned, Newsgroup>::iterator it = newsgroups.begin(); it != newsgroups.end(); it++){
			unsigned ng_id = it->first;
			string ng_name = it->second.getName();
			writeProtocolNbr(conn, ng_id);
			writeProtocolString(conn, ng_name);
		}
		conn->write(Protocol::ANS_END);
	}
}

//COM_CREATE_NG string_p COM_END
//ANS_CREATE_NG [ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS] ANS_END
void MessageController::createNG(Connection * conn) {
	string message = readString(conn);
	if (conn->read() == Protocol::COM_END) {
		// NEW WAY OF USING DAO
		try {
			storage->createNg(message);
			conn->write(Protocol::ANS_CREATE_NG);
			conn->write(Protocol::ANS_ACK);
			cout << "Newsgroup: " << message << " created" << endl;
		} catch (newsgroup_already_exists e) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
			cerr << "Newsgroup creation: " << message << " failed" << endl;
		}

		conn->write(Protocol::ANS_END);
	} else { // missing end command
		cerr << "Protocol failure: Missing End command" << endl;
	}
}

//COM_DELETE_NG num_p COM_END
//ANS_DELETE_NG [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::deleteNG(Connection * conn) {
		unsigned int ng_id = readProtocolNbr(conn);
		cout << "NG ID IS: " << ng_id << endl;
		if (conn->read() == Protocol::COM_END) {
			// NEW WAY OF USING DAO
			try {
				cout << "BEFORE deleteNG" << endl;
				storage->deleteNg(ng_id);
				cout << "AFTER deleteNG" << endl;
				conn->write(Protocol::ANS_DELETE_NG);
				conn->write(Protocol::ANS_ACK);
				cout << "Newsgroup with id: " << ng_id << " deleted" << endl;
			} catch (newsgroup_already_exists e) {
				conn->write(Protocol::ANS_NAK);
				conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
				cerr << "Failed to delete newsgroup with id" << ng_id << endl;
			}

			conn->write(Protocol::ANS_END);
		}else { // missing end command
			cerr << "Protocol failure: Missing End command" << endl;
	}
}

//COM_LIST_ART num_p COM_END
//ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::listArt(Connection * conn) {
	unsigned int ng_id = readProtocolNbr(conn);
	if (conn->read() == Protocol::COM_END) {
		cout<<"Got COM_END"<<endl;
		try {
			conn->write(Protocol::ANS_LIST_ART);
			map<unsigned, Article> articles = storage->listArticlesInNg(ng_id);
			conn->write(Protocol::ANS_ACK);
			writeProtocolNbr(conn, articles.size());
			for (map<unsigned, Article>::iterator it = articles.begin(); it != articles.end(); it++){
				unsigned art_id = it->first;
				string art_name = it->second.getName();
				writeProtocolNbr(conn, art_id);
				writeProtocolString(conn, art_name);
			}
		} catch (newsgroup_already_exists e) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
			cerr << "Failed to list articles in newsgroup with id" << ng_id << endl;
		}
		conn->write(Protocol::ANS_END);
	}else { // missing end command

	}
}
//COM_CREATE_ART num_p string_p string_p string_p COM_END
//ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::createArt(Connection * conn) {
	unsigned int ng_id = readProtocolNbr(conn);
	string title=readProtocolString(conn);
	string author=readProtocolString(conn);
	string text=readProtocolString(conn);
	if(conn->read()!=Protocol::COM_END){
		cerr << "Protocol failure: Missing End command" << endl;
	}else{
		conn->write(Protocol::ANS_CREATE_ART);
		//boolean from create Article is needed
		storage->createArticle(ng_id,title,author,text);
		//conn->write(Protocol::ANS_NAK);
//		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);

		cout<<"Article with title: "<<title<<" created"<<endl;
		conn->write(Protocol::ANS_ACK);
		conn->write(Protocol::ANS_END);
	}
}
//COM_DELETE_ART num_p num_p COM_END
//ANS_DELETE_ART [ANS_ACK | ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void MessageController::deleteArt(Connection * conn) {
	unsigned int ng_id = readProtocolNbr(conn);
	unsigned int art_id = readProtocolNbr(conn);
	conn->write(Protocol::ANS_DELETE_ART);
	if(conn->read()!=Protocol::COM_END){
			cerr << "Protocol failure: Missing End command" << endl;
	}else{
			//NAK is never sent because there is no functionality in deleteArt to know wether the article was succesfully
		//deleted or not. We also need to obtain what caused the problem, the art_id or the ng_id
			storage->deleteArticle(art_id,ng_id);
			cout<<"Article with id: "<<art_id<<" deleted"<<endl;
			conn->write(Protocol::ANS_ACK);
	}
	conn->write(Protocol::ANS_END);
}
//COM_GET_ART num_p num_p COM_END
//ANS_GET_ART [ANS_ACK string_p string_p string_p |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void MessageController::getArt(Connection * conn) {
	unsigned int ng_id = readProtocolNbr(conn);
	unsigned int art_id = readProtocolNbr(conn);
	if(conn->read()!=Protocol::COM_END){
		cerr << "Protocol failure: Missing End command" << endl;
	}else{
		conn->write(Protocol::ANS_GET_ART);
		Article a=storage->findArticle(art_id,ng_id);
		if(a.getId()!=0){ //ERROR, schould be a!=0?
			conn->write(Protocol::ANS_ACK);
			//title, aut, text
			writeProtocolString(conn,a.getName());
			writeProtocolString(conn,a.getAuthor());
			writeProtocolString(conn,a.getText());
		}else{
			conn->write(Protocol::ANS_NAK);
			//Decide what the problem is and send the right nak!
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
	}
}

}
