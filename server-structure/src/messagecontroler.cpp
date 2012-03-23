/*
 * messagemontroler.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#include "messagecontroler.h"
#include "storageinterface.h"
#include "core/protocol.h"
#include "core/connection.h"
#include <algorithm>
#include <utility>
#include <string>

using namespace std;
using namespace protocol;
using namespace client_server;

namespace client_server {

	// MAYBE WE SHOULD BREAK OUT THE "HELP"-FUNCTIONS IN THE BOTTOM AND PUT THEM IN A DECODER/ENCODER CLASS
	// INSTEAD SO THOSE FUNCTIONS DOES NOT DEPEND ON IF IT IS SERVER OR CLIENT???

	Connection* conn;
	StorageInterface* db;

	MessageControler::MessageControler(Connection* conn, StorageInterface* database) : conn(conn), db(database){
		int command = readNumber();
		switch(command) {
		case Protocol::COM_LIST_NG:
			MessageControler::list_ng();
			break;
		case Protocol::COM_CREATE_NG:
			MessageControler::create_ng();
			break;
		case Protocol::COM_DELETE_NG:
			MessageControler::delete_ng();
			break;
		case Protocol::COM_LIST_ART:
			MessageControler::list_art();
			break;
		case Protocol::COM_CREATE_ART:
			MessageControler::create_art();
			break;
		case Protocol::COM_DELETE_ART:
			MessageControler::delete_art();
			break;
		case Protocol::COM_GET_ART:
			MessageControler::get_art();
			break;
		}

	}

	MessageControler::~MessageControler() {
		// TODO Auto-generated destructor stub
	}

	void MessageControler::list_ng(){
		//Change so it reads a byte instead of an int... or else I think it will fuck up if it gets a char..
		int secCommand = readNumber();
		if(secCommand != Protocol::COM_END){
			// throw some exception... TODO
		}
		else{
			sendInt(Protocol::ANS_LIST_NG);
			sendInt(Protocol::PAR_NUM);
			//Move this implementation to storagememory.cpp?!
			map<unsigned, Newsgroup> newsgroups = db->listNg();
			map<unsigned, Newsgroup>::iterator it = newsgroups.begin();
			sendInt(newsgroups.size());
			for (it; it != newsgroups.end(); it++){
				unsigned ng_id = *it->first;
				string ng_name = *it->second.getName();
				sendInt(Protocol::PAR_NUM);
				sendInt(ng_id);
				sendInt(Protocol::PAR_STRING);
				sendInt(ng_name.size());
				writeString(ng_name);
			}
			sendInt(Protocol::ANS_END);
		}
	}

	void MessageControler::create_ng(){
		int secCommand = readNumber();
		if(secCommand != Protocol::PAR_STRING){
			// throw some exception... TODO
		}
		else{
			string ng_name = recvStringParam();
			int thirdCommand = readNumber();
			if(thirdCommand != Protocol::COM_END){
				// throw some exception... TODO
			}
			sendInt(Protocol::ANS_CREATE_NG);
			int n = db->createNg(ng_name);
			// TODO IMPLEMENTERA DENNA, FIXA FÖRST I db->createNg
			//if(n == NÅGOT){
			//	sendInt(Protocol::ANS_ACK);
			//}
			//else{
			//	sendInt(Protocol::ANS_NAK);
			//	sendInt(Protocol::ERR_NG_ALREADY_EXISTS);
			//}
			sendInt(Protocol::ANS_END);
		}
	}

	void MessageControler::delete_ng(){
		// TODO implement this!
	}

	void MessageControler::list_art(){
		// TODO implement this!
	}

	void MessageControler::create_art(){
		// TODO implement this!
	}

	void MessageControler::delete_art(){
		// TODO implement this!
	}

	void MessageControler::get_art(){
		// TODO implement this!
	}

	/*
	 * Read a number from the connection, four bytes (32-bit Integer)
	 * readNumber
	 */
	int readNumber() throw (ConnectionClosedException) {
		unsigned char byte1 = conn->read();
		unsigned char byte2 = conn->read();
		unsigned char byte3 = conn->read();
		unsigned char byte4 = conn->read();
		return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
	}

	/*
	 * Server, server output function
	 */
	void writeString(const string& s) throw (ConnectionClosedException) {
		for (size_t i = 0; i < s.size(); i) {
			conn->write(s[i]);
		}
		conn->write('$');
		// â€˜$â€™ is end of string
	}

	void sendInt(int value){
		// TODO implement this!
	}

	string recvStringParam(){
		int ng_namesize = readNumber();
		string name = 0;
		for(int i=0; i<ng_namesize; i++){
			name += conn->read();
		}
		return name;
	}
}
