/*
 * StorageInterface.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef STORAGEINTERFACE_H_
#define STORAGEINTERFACE_H_

#include "../entities/article.h"
#include "../entities/newsgroup.h"
#include "../entities/user.h"
#include <string>
#include <map>
using namespace std;

namespace server_client{
class Article;
class Newsgroup;
class User;

class StorageInterface {
	public:
		virtual ~StorageInterface(){};
		virtual Article findArticle(int $id, Newsgroup & ng) =0;
		virtual Article* createArticle(string name, User user, string text) = 0;
		virtual Article* deleteArticle(int id) = 0;
		virtual map<unsigned int, Article>* listArticles() = 0;
		virtual Newsgroup* createNg(string) = 0;
		virtual Newsgroup* deleteNg(unsigned int) = 0;
		virtual map<unsigned int, Newsgroup>* listNg() = 0;
	};
}
#endif /* STORAGEINTERFACE_H_ */
