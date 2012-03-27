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

#include <assert.h>

#include "core/connection.h"
#include "core/protocol.h"
#include "core/server.h"
#include "core/connectionclosedexception.h"
#include "dao/storageinterface.h"
#include "dao/storagefactory.h"
#include "messagecontroller.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;
using namespace client_server;
using namespace protocol;

void testStorageMemory() {
	cout << "Welcome to SERVER Frederik v 0.001" << endl;

	// setting up storage for data using Factory Pattern
	// options : memory || database
	StorageInterface * storage = StorageFactory::getDAO("memory"); // other option database for using filesystem

	// create and list Newsgroup test
	 storage->createNg("NG 1");
	 storage->createNg("NG 2");
	 storage->createNg("NG 3");


	try{
		storage->createNg("NG 3"); // duplicity is not added
		assert( ( storage->listNg().size() == 3 )); // size should be 3 NG
		} catch (newsgroup_already_exists e){
			assert(true); // I am here
		}
	// Testing of deleting NG
	try{
		storage->deleteNg(10);
	}catch(newsgroup_doesnt_exists e){
		cout << "DELETE NG: NG doesn't exists" << endl;
	}

	// Articles in the list TEST1
	try{
	assert (storage->listArticlesInNg(1).size() == 0); // there are no articles
	assert (storage->listArticlesInNg(10).size() == 0); // there are no articles

	} catch (newsgroup_doesnt_exists e){
		assert(true); // NG number 10 doesnt exist and invokes exception

	}

	// Creating articles
	assert( storage->createArticle(3,  "Title 1  / in 3", "Josef", "This is the text.") == 1 );
	assert( storage->createArticle(3,  "Title 2  / in 3","Bjorn", "This is the text.") == 2 );
	assert( storage->createArticle(2,  "Title 3 / in 2","Chriss","This is the text.")== 3 );

		assert(storage->listArticlesInNg(3).size() == 2);  // 2 articles in this NG
		assert(storage->listArticlesInNg(2).size() == 1);  // 1 articles in this NG
		assert(storage->listArticlesInNg(1).size() == 0);  // 0 articles in this NG

	assert(storage->listArticles().size() == 3); // We have 3 articles in all NG

	// Delete article
	try{
		storage->deleteArticle(1,3);
	//	storage->deleteArticle(55,3); // unexisting article
		storage->deleteArticle(2,5); // unexisting category


	}
	catch (newsgroup_doesnt_exists e){
		cout << "DELETE Article: Newsgroup doesn't exists"  << endl;
	}
	catch (article_doesnt_exists e){
		cout << "DELETE Article: Article doesn't exists"  << endl;
	}

	// Find Article Test
	try{
		Article test(1,3,"Title Test","Author","Text of article");
		Article& article = storage->findArticle(1,3);
		assert (test == article); // assert is comparing only id, not NG, its not necessary, every article has unique id

		//storage->findArticle(1,4);
		storage->findArticle(34,3); // second combination, where articel doesn't exists
	}
	catch (newsgroup_doesnt_exists e){
		cout << "FIND Article: Newsgroup doesn't exists";
	}
	catch (article_doesnt_exists e){
		cout << "FIND Article: Article doesn't exists";
	}

	// Delete NG test
	try{
	//storage->debugPrint();
	 storage->deleteNg(3); // 2 articles in this group shoud be deleted
	 storage->deleteNg(1); // 1 articles in this group shoud be deleted
	 storage->deleteNg(2);  // 0 articles in this group shoud be deleted

	 //following will throw an newsgroup_doesnt_exists
	 storage->deleteNg(2) ; // Non-existing NG
	 storage->deleteNg(22); // Non-existing NG
	//storage->debugPrint();
	} catch(newsgroup_doesnt_exists e) {
		//cout << storage->listNg().size() << " number of NG" << endl;
		assert(storage->listNg().size() == 0);
		//cout << storage->listArticles().size() << " number of Articles" << endl;
		assert(storage->listArticles().size() == 0);
	}

}

void testStorageDisk() {
	cout << "Welcome to SERVER Frederik v 0.001 / DISK STORAGE" << endl;

	// setting up storage for data using Factory Pattern
	// options : memory || database
	StorageInterface * storage = StorageFactory::getDAO("disk"); // other option database for using filesystem

	// create and list Newsgroup test
	//storage->createNg("NG 1");
	 //storage->createNg("NG 2");
	//storage->createNg("NG 3");

}


int main(int argc, char* argv[]) {
	MessageController mc;
	//testStorageMemory();
//	testStorageDisk();




	const int defaultPort = 30004;
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
				int command = conn->read();
				mc.execute(command, conn);
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

