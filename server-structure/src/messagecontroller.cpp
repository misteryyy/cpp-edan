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
using namespace protocol;
using namespace std;
namespace client_server {

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

void MessageController::execute(int command, string& message,
		Connection * conn) {
	switch (command) {

	case Protocol::COM_LIST_NG:
		listNG(conn);
		break;
	case Protocol::COM_CREATE_NG:
		createNG(message, conn);
		break;
	case Protocol::COM_DELETE_NG:
		deleteNG(message, conn);
		break;
	case Protocol::COM_LIST_ART:
		listArt(message, conn);
		break;
	case Protocol::COM_CREATE_ART:
		createArt(message, conn);
		break;
	case Protocol::COM_DELETE_ART:
		deleteArt(message, conn);
		break;
	case Protocol::COM_GET_ART:
		getArt(message, conn);
		break;

	default:
		break;

	}
}

void MessageController::listNG(Connection * conn){}

void MessageController::createNG(string& message, Connection * conn){
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

void MessageController::deleteNG(string& message, Connection * conn){}
void MessageController::listArt(string& message, Connection * conn){}
void MessageController::createArt(string& message, Connection * conn){}
void MessageController::deleteArt(string& message, Connection * conn){}
void MessageController::getArt(string& message, Connection * conn){}


}
