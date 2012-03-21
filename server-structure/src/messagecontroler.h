/*
 * messagemontroler.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef MESSAGEMONTROLER_H_
#define MESSAGEMONTROLER_H_

#include <string>
#include "../core/protocol.h"

using namespace std;

namespace client_server {

class MessageControler {
public:
	MessageControler(Connection conn);
	virtual ~MessageControler();
	// Change void to something...
	void list_ng();	           // list newsgroups
	void create_ng();          // create newsgroup
	void delete_ng();          // delete newsgroup
	void list_art();           // list articles
	void create_art();         // create article
	void delete_art();         // delete article
	void get_art();		       // get article

};

} /* namespace client_server */
#endif /* MESSAGEMONTROLER_H_ */

// DONT THINK WE SHOULD DO THIS IN HERE, OR IF WE EVEN NEED IT... COMMENT IT OUT FOR NOW!
// THIS IS FROM MESSAGEHANDLER.JAVA
/*
void sendByte(int code);
void sendCode(int code); // Transmit a code (a constant from the Protocol class).
void sendInt(int value); // Transmit an int value, according to the protocol.
void sendIntParameter(int param); // Transmit an int parameter, according to the protocol.
void sendStringParameter(string param); // Transmit a string parameter, according to the protocol.
int recvByte();
int recvCode(); // Receive a command code or an error code from the server.
int recvInt(); // Receive an int value from the server.
int recvIntParameter(); // Receive an int parameter from the server.
string recvStringParameter(); // Receive a string parameter from the server.
*/
