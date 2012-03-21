//============================================================================
// Name        : server-run.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : This is the server implementation.
//============================================================================

#include <iostream>
#include <string>
#include <cstdlib>

#include "core/connection.h"
#include "core/server.h"
#include "core/connectionclosedexception.h"
#include "dao/storageinterface.h"

using namespace std;
using namespace client_server;

/*
 * Main function of Server
 */
int main(int argc, char* argv[]){
	if(argc != 2){
		cerr << "Usage: myserver port-number " << endl;
		//exit(1);
	}

	// For debug we can set this for some numbers -> 1024+
	// There is problem, that if the program is not closed
	// properly, we have to use other socket or kill the
	// server manualy in the service manager. I guess :D // josef

	Server server(1301);

	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}

	//reading data from clients in cycle
	while(true){
		Connection *conn = server.waitForActivity();
		// new connection with client is establish
		if(conn != 0){
			cout << "Connection " << conn << " established";
			try{

				//response server login -> MessageController

			}catch (ConnectionClosedException&){
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		}
		else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
	}
	server.~Server();
	return 0;
}


