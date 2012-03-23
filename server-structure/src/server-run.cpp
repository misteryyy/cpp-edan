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
#include "messagecontroler.h"

using namespace std;
using namespace client_server;

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
	MessageControler mc = 0;
	while (true) {
		Connection * conn = server.waitForActivity();
		// new connection with client is establish
		if (conn != 0) {
			cout << "Connection " << conn << " established";
			try {
				//response server login > MessageController
				 mc = MessageControler::MessageControler(*conn);

			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		}
		else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
		server.~Server();
	}
	return 0;
}

