/* myclient.cc: sample client program */

#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using client_server::Connection;
using client_server::ConnectionClosedException;


/*
 * Send the integer 'value' to the server.
 */
void writeNumber(int value, Connection& conn) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string readString(Connection& conn) {
    string s;
    char ch;
    while ((ch = conn.read()) != '$')  // '$' is end of string
        s += ch;
    return s;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
    
    Connection conn(argv[1], atoi(argv[2]));
    if (! conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    cout << "Type a number: ";
    int nbr;
    while (cin >> nbr) {
        try {
            writeNumber(nbr, conn);
            string answer = readString(conn);
            cout << nbr << " is " << answer << endl;
            cout << "Type another number: ";
        } 
        catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            exit(1);
        }
    }
}

