/*
 * storagememory.h
 *
 *  Created on: 20 mar 2012
 *      Author: Josef
 */

#ifndef STORAGEMEMORY_H_
#define STORAGEMEMORY_H_

#include "../entities/article.h"
#include "../entities/newsgroup.h"
#include "storageinterface.h"
#include <map>
#include <string>

using namespace std;
namespace client_server{

class StorageMemory : public StorageInterface {

public:
		StorageMemory(){};
		~StorageMemory(){};
		/**
		* Newsgroup methods
		*/
		virtual void createNg(const string &) throw(newsgroup_already_exists);
		virtual void deleteNg(unsigned int) throw(newsgroup_doesnt_exists);
		virtual ng_map_type& listNg();


		/**
		 * Article methods
		 */
	 	virtual art_map_type listArticlesInNg(signed ng_id) throw(newsgroup_doesnt_exists);
		virtual int createArticle(int ng_id, const string & title ,const string & author,const string & text) throw(newsgroup_doesnt_exists);
	  	virtual void deleteArticle(int id, int ng_id)  throw(newsgroup_doesnt_exists,article_doesnt_exists);
	  	virtual Article& findArticle(int id,int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists) ; // return reference to Article
	  	virtual art_map_type& listArticles();

	  	// not implemented in StorageInterface - for internal usage
	 	int testArticleId(int id, int ng_id); // tests if the article ID exists
	 	int findNg(const string &); // return 0 if no match
	 	int findNg(int ng_id);

	 	/**
	 	 * Debugging methods
	 	 */
	 	virtual void debugPrint() const;

private:
	ng_map_type newsgroups; // list of newsgroup
	art_map_type articles; // list of articles
	static int lastNgId,lastArtId; // static variable which remember the last id of newsgroup

};

} /* namespace server_client */
#endif /* STORAGEMEMORY_H_ */
