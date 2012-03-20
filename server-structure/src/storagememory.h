/*
 * storagememory.h
 *
 *  Created on: 20 mar 2012
 *      Author: Chris
 */

#ifndef STORAGEMEMORY_H_
#define STORAGEMEMORY_H_

#include <map>

using namespace std;

namespace server_client {

class storagememory {
public:
	storagememory();
	virtual ~storagememory();
private:
	//Some datastructure... think a map is the best...
	map<unsigned int, Newsgroup> newsgroups;
};

} /* namespace server_client */
#endif /* STORAGEMEMORY_H_ */
