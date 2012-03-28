/*
 * protocolhandler.h
 *
 *  Created on: Mar 28, 2012
 *      Author: buffalo
 */

#ifndef PROTOCOLHANDLER_H_
#define PROTOCOLHANDLER_H_
#include <string>
#include "connection.h"
using namespace std;
namespace client_server {
class ProtocolHandler {
public:
	ProtocolHandler(const Connection& c):conn(c){}
	int readNumber() throw (ConnectionClosedException);
	string readProtocolString();
	void writeProtocolNbr(int val);
	void writeProtocolString(string const& mess);
	int readProtocolNbr();
	void writeNumber(int value);
	void writeChar(unsigned char b);
	unsigned char readChar();
	virtual ~ProtocolHandler();
private:
	Connection conn;
};

}
#endif /* PROTOCOLHANDLER_H_ */
