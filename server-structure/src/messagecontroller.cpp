/*
 * messagecontroller.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: buffalo
 */

#include "messagecontroller.h"
#include "core/protocol.h"
#include "core/connection.h"
#include "core/server.h"
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

int readProtocolNbr(Connection * conn) {
	if (conn->read() == Protocol::PAR_NUM) {
		int nbr = readNumber(conn);
		return nbr;
	} else {
		cerr << "Protocol failure: Bad command, expected PAR_NUM" << endl;
		// CLOSE DOWN THE CLIENT
	}
	return 0;
}

string readProtocolString(Connection * conn) {
	if (conn->read() == Protocol::PAR_STRING) {
		int nbr = readNumber(conn);
		string mess;
		for (int i = 0; i < nbr; i++) {
			mess += conn->read();
		}
		return mess;
	} else {
		cerr << "Protocol failure: Bad command, expected PAR_STRING" << endl;
		// CLOSE DOWN THE CLIENT
	}
	return 0;
}

MessageController::MessageController(){
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

/*
void MessageController::closeConnection(Connection * conn){
	cerr << "Closing connection to server." << endl;
	server.deregisterConnection(conn);
	delete conn;
}
*/

void MessageController::execute(int command, Connection * conn, Server * server){

	switch (command) {

	case Protocol::COM_LIST_NG:
		listNG(conn, server);
		break;
	case Protocol::COM_CREATE_NG:
		cout << "CREATE NG" << endl;
		createNG(conn, server);
		break;
	case Protocol::COM_DELETE_NG:
		deleteNG(conn, server);
		break;
	case Protocol::COM_LIST_ART:
		cout << "LIST ART" << endl;
		listArt(conn, server);
		break;
	case Protocol::COM_CREATE_ART:
		createArt(conn, server);
		break;
	case Protocol::COM_DELETE_ART:
		deleteArt(conn, server);
		break;
	case Protocol::COM_GET_ART:
		getArt(conn, server);
		break;

	default:
		cerr << "Protocol failure: Bad command, expected a COM_ type of command" << endl;
		cerr << "Closing connection to server." << endl;
			server->deregisterConnection(conn);
			delete conn;
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
void MessageController::listNG(Connection * conn, Server * server) {
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_LIST_NG);
		map<unsigned, Newsgroup> newsgroups = storage->listNg();
		writeProtocolNbr(conn, newsgroups.size());
		for (map<unsigned, Newsgroup>::iterator it = newsgroups.begin();
				it != newsgroups.end(); it++) {
			//unsigned ng_id = it->first;
			//string ng_name = it->second.getName();
			writeProtocolNbr(conn, it->first);
			writeProtocolString(conn, it->second.getName());
		}
		conn->write(Protocol::ANS_END);
	} else {
		cerr << "Listing newsgroups failed" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_CREATE_NG string_p COM_END
//ANS_CREATE_NG [ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS] ANS_END
void MessageController::createNG(Connection * conn, Server * server) {
	string message = readString(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_CREATE_NG);
		try {
			storage->createNg(message);
			conn->write(Protocol::ANS_ACK);
			//Delete cout!!!
			cout << "Newsgroup: " << message << " created" << endl;
		} catch (newsgroup_already_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
			//Delete cerr!!!
			cerr << "Newsgroup creation: " << message << " failed" << endl;
		}

		conn->write(Protocol::ANS_END);
	} else { // missing end command
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_DELETE_NG num_p COM_END
//ANS_DELETE_NG [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::deleteNG(Connection * conn, Server * server) {
	unsigned int ng_id = readProtocolNbr(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_DELETE_NG);
		try {
			storage->deleteNg(ng_id);
			conn->write(Protocol::ANS_ACK);
			//Delete cout!!!
			cout << "Newsgroup with id: " << ng_id << " deleted" << endl;
		} catch (newsgroup_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
			// Delete cerr!!
			cerr << "Failed to delete newsgroup with id" << ng_id << endl;
		}
		conn->write(Protocol::ANS_END);
	} else { // missing end command
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_LIST_ART num_p COM_END
//ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::listArt(Connection * conn, Server * server) {
	unsigned int ng_id = readProtocolNbr(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_LIST_ART);
		try {
			map<unsigned, Article> articles = storage->listArticlesInNg(ng_id);
			conn->write(Protocol::ANS_ACK);
			writeProtocolNbr(conn, articles.size());
			for (map<unsigned, Article>::iterator it = articles.begin();
					it != articles.end(); it++) {
				//unsigned art_id = it->first;
				//string art_name = it->second.getName();
				writeProtocolNbr(conn, it->first);
				writeProtocolString(conn, it->second.getName());
			}
		} catch (newsgroup_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
			// Delete cerr!!
			cerr << "Failed to list articles in newsgroup with id" << ng_id
					<< endl;
		}
		conn->write(Protocol::ANS_END);
	} else { // missing end command
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_CREATE_ART num_p string_p string_p string_p COM_END
//ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
void MessageController::createArt(Connection * conn, Server * server) {
	unsigned int ng_id = readProtocolNbr(conn);
	string title = readProtocolString(conn);
	string author = readProtocolString(conn);
	string text = readProtocolString(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_CREATE_ART);
		try {
			storage->createArticle(ng_id, title, author, text);
			// Delete cout!!
			cout << "Article with title: " << title << " created" << endl;
			conn->write(Protocol::ANS_ACK);
		} catch (newsgroup_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
	} else {
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_DELETE_ART num_p num_p COM_END
//ANS_DELETE_ART [ANS_ACK | ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void MessageController::deleteArt(Connection * conn, Server * server) {
	unsigned int ng_id = readProtocolNbr(conn);
	unsigned int art_id = readProtocolNbr(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_DELETE_ART);
		try {
			//NAK is never sent because there is no functionality in deleteArt to know wether the article was succesfully
			//deleted or not. We also need to obtain what caused the problem, the art_id or the ng_id
			storage->deleteArticle(art_id, ng_id);
			// Delete cout!!
			cout << "Article with id: " << art_id << " deleted" << endl;
			conn->write(Protocol::ANS_ACK);
		} catch (newsgroup_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		} catch (article_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
	} else {
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}

//COM_GET_ART num_p num_p COM_END
//ANS_GET_ART [ANS_ACK string_p string_p string_p |ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
void MessageController::getArt(Connection * conn, Server * server) {
	unsigned int ng_id = readProtocolNbr(conn);
	unsigned int art_id = readProtocolNbr(conn);
	if (conn->read() == Protocol::COM_END) {
		conn->write(Protocol::ANS_GET_ART);
		try {
			Article art = storage->findArticle(art_id, ng_id);
			conn->write(Protocol::ANS_ACK);
			// title, author, text
			writeProtocolString(conn, art.getName());
			writeProtocolString(conn, art.getAuthor());
			writeProtocolString(conn, art.getText());
		} catch (newsgroup_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		} catch (article_doesnt_exists&) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
	} else {
		cerr << "Protocol failure: Missing End command" << endl;
		cerr << "Closing connection to server." << endl;
		server->deregisterConnection(conn);
		delete conn;
	}
}
}
