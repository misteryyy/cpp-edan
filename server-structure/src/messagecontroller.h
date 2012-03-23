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
	void execute(int command, Connection* conn);
	void writeString(const string& s, Connection * conn)throw (ConnectionClosedException);
	string readString(Connection* conn);
	void listNG(Connection * conn);
	void createNG(Connection * conn);
	void deleteNG(Connection * conn);
	void listArt( Connection * conn);
	void createArt(Connection * conn);
	void deleteArt( Connection * conn);
	void getArt(Connection * conn);

private:
 	 StorageInterface * storage;
};
}
#endif /* MESSAGECONTROLLER_H_ */
