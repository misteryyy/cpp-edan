/*
 * storagememory.h
 *
 *  Created on: 20 mar 2012
 *      Author: Chris
 */

#ifndef STORAGEMEMORY_H_
#define STORAGEMEMORY_H_

#include "../entities/article.h"
#include "../entities/newsgroup.h"
#include "../entities/user.h"
#include <map>

#include <string>


using namespace std;

namespace client_server{

class StorageMemory {

public:
	StorageMemory();
	 ~StorageMemory();
private:
	//Some datastructure... think a map is the best...
	map<unsigned int, Newsgroup> newsgroups;
};

} /* namespace server_client */
#endif /* STORAGEMEMORY_H_ */
