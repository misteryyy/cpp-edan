/*
 * storagefactory.h
 *
 *  Created on: Mar 20, 2012
 *      Author: misteryyy
 */

#ifndef STORAGEFACTORY_H_
#define STORAGEFACTORY_H_

using namespace std;
#include "storageinterface.h"
#include "storagememory.h"
#include "storagedisk.h"

namespace client_server{


class StorageFactory{
public:
	static StorageInterface * getDAO(const string &type ){


		if(type == "memory"){
				return new StorageMemory;
			}
			if(type == "disk"){
				return new StorageDisk;
			}
			return 0;
	}

};
}
#endif /* STORAGEFACTORY_H_ */
