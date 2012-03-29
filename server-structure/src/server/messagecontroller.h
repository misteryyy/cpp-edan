/*
 * messagecontroller.h
 *
 *  Created on: Mar 23, 2012
 *      Author: buffalo
 */
#include "../dao/storageinterface.h"
#include "../dao/storagefactory.h"
#include "../core/connection.h"
#include "../core/server.h"

#ifndef MESSAGECONTROLLER_H_
#define MESSAGECONTROLLER_H_
namespace client_server {
class MessageController {
public:
	MessageController(const string& s);
	virtual ~MessageController();
	//void closeConnection(Connection * conn);
	void execute(int command, Connection* conn, Server* server);
	void writeString(const string& s, Connection * conn)throw (ConnectionClosedException);
	string readString(Connection* conn);
	void listNG(Connection * conn, Server * server);
	void createNG(Connection * conn, Server * server);
	void deleteNG(Connection * conn, Server * server);
	void listArt( Connection * conn, Server * server);
	void createArt(Connection * conn, Server * server);
	void deleteArt( Connection * conn, Server * server);
	void getArt(Connection * conn, Server * server);

private:
	StorageInterface * storage;
};
}
#endif /* MESSAGECONTROLLER_H_ */
