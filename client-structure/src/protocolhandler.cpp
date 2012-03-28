/*
 * protocolhandler.cpp
 *
 *  Created on: Mar 28, 2012
 *      Author: buffalo
 */

#include "protocolhandler.h"
#include "protocol.h"
#include <string>
#include <iostream>
using namespace protocol;
using namespace std;
namespace client_server {
int ProtocolHandler::readNumber() throw (ConnectionClosedException) {
	unsigned char byte1 = conn.read();
	unsigned char byte2 = conn.read();
	unsigned char byte3 = conn.read();
	unsigned char byte4 = conn.read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
void ProtocolHandler::writeNumber(int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}
void ProtocolHandler::writeProtocolString(string const& mess) {
	conn.write(Protocol::PAR_STRING);
	int len = mess.size();
	writeNumber(len);
	for (int i = 0; i < len; i++) {
		conn.write(mess[i]);
	}
}
void ProtocolHandler::writeProtocolNbr(int val) {
	conn.write(Protocol::PAR_NUM);
	writeNumber(val);
}

int ProtocolHandler::readProtocolNbr(){
	if(conn.read() == Protocol::PAR_NUM){
		int nbr = readNumber();
		return nbr;
	}
	else{
		cerr << "Protocol failure: Bad command, expected PAR_NUM" << endl;
	}
		return 0;
		//Maybe should be system exit here!
}
string ProtocolHandler::readProtocolString(){
	if(conn.read() == Protocol::PAR_STRING){
		int nbr = readNumber();
		string mess;
		for(int i=0;i<nbr;i++){
			mess+=conn.read();
		}
		return mess;
	}
	else{
		cerr << "Protocol failure: Bad command, expected PAR_STRING" << endl;
	}
		return 0;
		//Maybe should be system exit here!
}
void ProtocolHandler::writeChar(unsigned char ch){
	conn.write(ch);
}
unsigned char ProtocolHandler::readChar(){
	return conn.read();
}
ProtocolHandler::~ProtocolHandler() {
}
}
