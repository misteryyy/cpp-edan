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

MessageController::MessageController() {
	storage = StorageFactory::getDAO("memory");
}

MessageController::~MessageController() {
	// TODO Auto-generated destructor stub
}

void MessageController::writeString(const string& s, Connection * conn)
		throw (ConnectionClosedException) {
	for (size_t i = 0; i < s.size(); i) {
		conn->write(s[i]);
	}
	conn->write('$');
	// ‘$’ is end of string
}

void MessageController::execute(int command, Connection * conn) {
	switch (command) {

	case Protocol::COM_LIST_NG:
		listNG(conn);
		break;
	case Protocol::COM_CREATE_NG:
		createNG(conn);
		break;
	case Protocol::COM_DELETE_NG:
		deleteNG(conn);
		break;
	case Protocol::COM_LIST_ART:
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

string MessageController::readString(Connection* conn){
	unsigned char type = conn->read();
	if(type != Protocol::PAR_STRING){
		cerr << "fail: " << type << endl;
	}
	int lengthOfMessage = readNumber(conn);
	unsigned char nbr;
	string mess;
	for(int i =0; i < lengthOfMessage; i++){
		stringstream ss;
		nbr = conn->read();
		//cout << nbr << endl;
		ss << mess << nbr;
		ss >> mess;
	}
	mess = mess.substr(0, mess.size() - 1);
	return mess;
}

void MessageController::listNG(Connection * conn){}

void MessageController::createNG(Connection * conn){
	string message;
	message= readString(conn);

	bool create;
	create = storage->createNg(message);
	writeString(Protocol::ANS_CREATE_NG+"", conn);
	if (create) {
		writeString(Protocol::ANS_ACK+"", conn);
	} else {
		writeString(Protocol::ANS_NAK+"", conn);
		writeString(Protocol::ERR_NG_ALREADY_EXISTS+"", conn);
	}
	writeString(Protocol::ANS_END+"", conn);
	cout << "Newsgroup: " << message << " created" << endl;
}

void MessageController::deleteNG(Connection * conn){}
void MessageController::listArt(Connection * conn){}
void MessageController::createArt(Connection * conn){}
void MessageController::deleteArt(Connection * conn){}
void MessageController::getArt(Connection * conn){}


}
