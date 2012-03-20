/*
 * StorageInterface.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef STORAGEINTERFACE_H_
#define STORAGEINTERFACE_H_

#include "entities/article.h"
#include "entities/newsgroup.h"
#include "entities/user.h"
#include <string>

using namespace std;

namespace server_client{
class Article;
class Newsgroup;
class User;

class StorageInterface {
	public:
		virtual StorageInterface(){};
		virtual ~StorageInterface(){};
		virtual Article findArticle(int $id, Newsgroup & ng);
		virtual Article* createArticle(string name, User user, string text);
		virtual Article* deleteArticle();
		virtual map<unsigned int, Article>* listArticles();
		virtual Newsgroup* createNg();
		virtual Newsgroup* deleteNg();
		virtual map<unsigned int, Newsgroup>* listNg();
	};
}
#endif /* STORAGEINTERFACE_H_ */
