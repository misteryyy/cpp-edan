/*
 * storagedisk.h
 *
 *  Created on: 20 mar 2012
 *      Author: Josef
 */

#ifndef STORAGEDISK_H_
#define STORAGEDISK_H_

#include "../entities/article.h"
#include "../entities/newsgroup.h"
#include "storageinterface.h"
#include <map>
#include <string>

using namespace std;

namespace client_server {


class StorageDisk : public StorageInterface {
public:
		StorageDisk();
		virtual ~StorageDisk();

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
   		virtual Article& findArticle(int id,int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists); // return reference to Article
   		virtual art_map_type& listArticles();

   	 	int testArticleId(int id, int ng_id); // tests if the article ID exists
   		int findNg(const string &); // return 0 if no match
   		int findNg(int ng_id);
   		void buildNgList();

   	 	/**
   		 * Debugging methods
   		 */
   		 virtual void debugPrint() const;
private:
   		ng_map_type newsgroups; // list of newsgroup
   		art_map_type articles; // list of articles
   		int lastNgId,lastArtId; // these values are loaded from the file, from the last save
   		string baseDir;
};

} /* namespace server_client */
#endif /* STORAGEDISK_H_ */
