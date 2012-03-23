//============================================================================
// Name        : testmyserver.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansistyle
//============================================================================

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

#include "core/connection.h"
#include "core/protocol.h"
#include "core/server.h"
#include "core/connectionclosedexception.h"
#include "dao/storageinterface.h"
#include "dao/storagefactory.h"
#include "messagecontroller.h"

using namespace std;
using namespace client_server;
using namespace protocol;

/*
 * Server function
 * readNumber
 */
int readNumber(Connection * conn) throw (ConnectionClosedException) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Server, server output function
 */


void testStorage() {
	cout << "Welcome to SERVER Frederik v 0.001" << endl;

	// setting up storage for data using Factory Pattern
	// options : memory || database
	StorageInterface * storage = StorageFactory::getDAO("memory"); // other option database for using filesystem
	/*
	 * Newsgroup management DEMO
	 */
	storage->createNg("Blamor 1");
	storage->createNg("Apple 2");
	storage->createNg("Apple 2");
	storage->createNg("Apppppp 3");
	//storage>debugPrint();
	storage->deleteNg(10); // false if index doesn't exists
	int n = storage->findNg("Blamor 0");

	//storage>debugPrint();

	// Creating articles
	storage->createArticle(3, "Josef", "Title Lala id1", "This is the text.");
	storage->createArticle(2, "Chriss", "Title Blabla id2",
			"This is the text.");
	storage->createArticle(3, "Bjorn", "Title Blabla id3", "This is the text.");

	storage->debugPrint();
	cout << storage->listArticlesInNg(3).size()
			<< " / shoudl be 2, because in 3 are 2 articles" << endl;
	cout << storage->listArticlesInNg(2).size()
			<< " / shoudl be 1, because in 2 is 1 article" << endl;

	// Deleting article
	storage->deleteArticle(1);
	cout << storage->listArticlesInNg(3).size()
			<< " / shoudl be 1, one item has been deleted " << endl;
	storage->debugPrint();

}

int readCommand(Connection* conn) {
	unsigned char nbr = conn->read();
	//cout << "Command: " << nbr << endl;
	conn->read();
	conn->read();
	conn->read();
	conn->read();
	conn->read();
	return nbr;
}
string readStringTillEnd(Connection* conn) {
	unsigned char nbr;
	string mess;
	while (nbr != Protocol::COM_END) {
		stringstream ss;
		nbr = conn->read();
		//cout << nbr << endl;
		ss << mess << nbr;
		ss >> mess;
	}
	mess = mess.substr(0, mess.size() - 1);
	return mess;
}

/*
 * Main function of Server
 */
int main(int argc, char* argv[]) {
	MessageController mc;
	const int defaultPort = 30002;
	if (argc != 2) {
		cerr << "No port defined. Usage: myserver portnumber " << endl;
		cout << "Using default port:" << defaultPort << " instead " << endl;
		//exit(1);
	}
	Server server(defaultPort);
	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}
	while (true) {
		Connection* conn = server.waitForActivity();
		if (conn != 0) {
			try {
				int command = readCommand(conn);
				string s = readStringTillEnd(conn);
				mc.execute(command, s, conn);
				//cout<<"Command was:"<<command<<"string was: "<<s<<endl;
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		} else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
	}
}

