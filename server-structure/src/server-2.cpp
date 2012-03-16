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

#include "connection.h"
#include "connectionclosedexception.h"
#include "storageinterface.h"


using namespace std;
using namespace client_server;

/*
 * Function which is writing to the connection stream
 */
void writeNumber(int value, const Connection& conn) throw(ConnectionClosedException){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Function which is reading text from the connection stream
 */
string readString(const Connection & conn) throw(ConnectionClosedException){
	string s;
	char ch;
	while ((ch = conn.read()) != '$'){ //'$' is end of string

		 s+= ch;
	}
	return s;
}

int main_client(int argc, char* argv[]){
	// testing number of parameters
	if(argc !=3){

		cerr << "Usage> myclient host-name port-number" << endl;
		return 1;
	}

	//Setting up the connection with Server
	Connection conn( argv[1],atoi(argv[2]) );

	if(! conn.isConnected()){
		cerr << "Connection attempt failed" << endl;
		return 1;
	}

	cout << "Type a number: ";
	int nbr;
	while (cin >> nbr){
		try{
			writeNumber(nbr,conn);
			string answer = readString(conn);
			cout << nbr << " is " << answer << endl;
			cout << "Type another number: ";



		} catch (ConnectionClosedException&){
			cerr << "Server closed down" << endl;
			exit(1);
		}

	}
	return 0;

}



/*
 * Main function of Server
 */
int main(){
	 cout<< "Welcome to SERVER Frederik v 0.001" << endl;

	return 0;
}


