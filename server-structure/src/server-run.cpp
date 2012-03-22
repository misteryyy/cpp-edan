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

#include "core/connection.h"
#include "core/server.h"
#include "core/connectionclosedexception.h"
#include "dao/storageinterface.h"
#include "dao/storagefactory.h"

using namespace std;
using namespace client_server;

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
void writeString(const string& s, Connection * conn)
		throw (ConnectionClosedException) {
	for (size_t i = 0; i < s.size(); i) {
		conn->write(s[i]);
	}
	conn->write('$');
	// ‘$’ is end of string
}

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
	storage->createArticle(2, "Chriss", "Title Blabla id2", "This is the text.");
	storage->createArticle(3, "Bjorn", "Title Blabla id3", "This is the text.");

	storage->debugPrint();
	cout << storage->listArticlesInNg(3).size() << " / shoudl be 2, because in 3 are 2 articles" << endl;
	cout << storage->listArticlesInNg(2).size() << " / shoudl be 1, because in 2 is 1 article" << endl;

	// Deleting article
	storage->deleteArticle(1);
	cout << storage->listArticlesInNg(3).size() << " / shoudl be 1, one item has been deleted " << endl;
	storage->debugPrint();

}

/*
 * Main function of Server
 */
int main(int argc, char* argv[]) {

	if (argc != 2) {
		cerr << "Usage: myserver portnumber " << endl;
		//exit(1);
	}

	// For debug we can set this for some numbers > 1024
	// There is problem, that if the program is not closed
	// properly, we have to use other socket or kill the
	// server manualy in the service manager. I guess :D // josef

	Server server(1301);

	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}
	cout << "Welcome to SERVER Frederik v 0.001" << endl;

	//reading data from clients in cycle
	while (true) {
		Connection * conn = server.waitForActivity();
		// new connection with client is establish
		if (conn != 0) {
			cout << "Connection " << conn << " established";
			try {
				int nbr = readNumber(conn);
				//response server login > MessageController

			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		}
		server.~Server();
	}
	return 0;
}

