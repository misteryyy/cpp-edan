/*
 * StorageInterface.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef STORAGEINTERFACE_H_
#define STORAGEINTERFACE_H_

using namespace std;

#include "entities/article.h"
#include "entities/newsgroup.h"
#include "entities/user.h"
#include <string>

namespace server_client{
class Article;
class Newsgroup;
class User;

class StorageInterface {
	public:
	StorageInterface(){};
	Article findById(int $id, Newsgroup & ng);
	Article* createArticle(string name, User user);

 };

 }
#endif /* STORAGEINTERFACE_H_ */
