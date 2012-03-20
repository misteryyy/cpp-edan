/*
 * newsserver.cpp
 *
 *  Created on: 19 mar 2012
 *      Author: Chris
 *
 *  Description: This is the server we should implement...
 *  			 It is a good idea to change the names and stuff later because this code is from the "myserver.cc"-example
 */

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}

	Server server(atoi(argv[1]));
	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}

	while (true) {
		Connection* conn = server.waitForActivity();
		if (conn != 0) {
				// Some code here
			try {
				// HERE SHOULD BE THE CODE FOR WHAT THE PROGRAM SHOULD DO AFTER THE CONNECTION IS SETUP!
				// IMPLEMENT HERE!
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
