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
		StorageInterface(){};
		Article findArticleById(int $id, Newsgroup & ng);
		Article* createArticle(string name, User user, string text);
		Article* deleteArticle();
		//Some datastructure* listArticles();
		Newsgroup* createNg();
		Newsgroup* deleteNg();
		//Some datastructure* listNg();
	};
}
#endif /* STORAGEINTERFACE_H_ */
