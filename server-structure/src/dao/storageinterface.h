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

// exceptions
struct article_doesnt_exists {};
struct newsgroup_already_exists {};
struct newsgroup_doesnt_exists {};


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
	virtual ~StorageInterface(){};

		/**
		 * Newsgroup methods
		 */
		virtual void createNg(const string &) throw(newsgroup_already_exists) = 0;
		virtual void deleteNg(unsigned int) throw(newsgroup_doesnt_exists) = 0;
		virtual ng_map_type& listNg() = 0;

		/**
		 * Article methods
		 */
		virtual art_map_type listArticlesInNg(signed ng_id) throw(newsgroup_doesnt_exists) =0;
		virtual int createArticle(int ng_id, const string & title ,const string & author,const string & text) throw(newsgroup_doesnt_exists) = 0;
		virtual void deleteArticle(int id, int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists)= 0;

		virtual Article& findArticle(int id,int ng_id)throw(newsgroup_doesnt_exists,article_doesnt_exists) = 0; // return reference to Article

		virtual art_map_type& listArticles() = 0;

		/**
		 * Debugging methods
		 */
		virtual void debugPrint() const = 0;
	};


}

#endif /* STORAGEINTERFACE_H_ */
