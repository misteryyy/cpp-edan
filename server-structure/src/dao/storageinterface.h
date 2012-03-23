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
#include <string>
#include <map>
using namespace std;

namespace client_server{
class Article;
class Newsgroup;

/*
 * Defininition of shortcuts for datatypes
 */
typedef pair<unsigned,Newsgroup> ng_pair;
typedef map<unsigned, Newsgroup> ng_map_type;
typedef map<unsigned, Article>   art_map_type;
typedef pair<unsigned,Article> art_pair;

class StorageInterface {
	public:

		virtual int findArticle(int id)=0;
		virtual int createArticle(int ng_id, const string & author ,const string & title,const string & text) = 0;
		virtual bool deleteArticle(int id) = 0;
		//virtual art_map_type& listArticles() = 0;
		virtual bool createNg(const string &) = 0;
		virtual bool deleteNg(unsigned int) = 0;
		virtual ng_map_type& listNg() = 0;
		virtual art_map_type& listArticlesInNg(signed ng_id) =0;
		virtual void debugPrint() const = 0;
		virtual int findNg(const string &) = 0; // return reference to ng
		virtual int findNg(int ng_id) = 0;
	};
}
#endif /* STORAGEINTERFACE_H_ */
