//============================================================================
// Name        : test-my-server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
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
 * Server function
 * readNumber
 */
int readNumber(Connection * conn) throw(ConnectionClosedException){
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
/*
 * Server, server output function
 */

void writeString(const string& s, Connection * conn) throw(ConnectionClosedException){
	for(size_t i=0; i < s.size(); ++i){
		conn->write(s[i]);
	}
	conn->write('$');
	// ‘$’ is end of string
}

/*
 * Main function of Server
 */
int main(int argc, char* argv[]){
	if(argc != 2){
		cerr << "Usage: myserver port-number " << endl;
		//exit(1);
	}

	//For debug we can set this for some numbers -> 1024+
	// There is problem, that if the program is not closed
	// properly, we have to use other socket or kill the
	// server manualy in the service manager. I guess :D // josef

	Server server(1301);

	if (!server.isReady()) {
	cerr << "Server initialization error" << endl;
	exit(1);
	}

	cout<< "Welcome to SERVER Frederik v 0.001" << endl;
	//reading data from clients in cycle
	while(true){

		Connection * conn = server.waitForActivity();
		// new connection with client is establish
		if(conn != 0){
			cout << "Connection " << conn << " established";
			try{
				int nbr = readNumber(conn);

				//response server login -> MessageController
				if(nbr > 0){
					writeString("Positive",conn);
				} else if (nbr == 0){
					writeString("Zero",conn);
				} else {
					writeString("Negative", conn);
				}

			}catch (ConnectionClosedException&){
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;

			}



		}




	}
	server.~Server();
	return 0;
}


