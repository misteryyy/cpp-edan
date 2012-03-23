/*
 * messagecontroller.h
 *
 *  Created on: Mar 23, 2012
 *      Author: buffalo
 */
#include "dao/storageinterface.h"
#include "dao/storagefactory.h"

#include "core/connection.h"
#ifndef MESSAGECONTROLLER_H_
#define MESSAGECONTROLLER_H_
namespace client_server {
class MessageController {
public:
	MessageController();
	virtual ~MessageController();
	void execute(int command, string& message, Connection* conn);
	void writeString(const string& s, Connection * conn)throw (ConnectionClosedException);
	void listNG(Connection * conn);
	void createNG(string& message, Connection * conn);
	void deleteNG(string& message, Connection * conn);
	void listArt(string& message, Connection * conn);
	void createArt(string& message, Connection * conn);
	void deleteArt(string& message, Connection * conn);
	void getArt(string& message, Connection * conn);

private:
 	 StorageInterface * storage;
};
}
#endif /* MESSAGECONTROLLER_H_ */
