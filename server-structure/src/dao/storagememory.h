/*
 * storagememory.h
 *
 *  Created on: 20 mar 2012
 *      Author: Chris
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
		virtual int findArticle(int id);
	 	virtual art_map_type& listArticlesInNg(signed ng_id);
	 	virtual int createArticle(int ng_id, const string & author ,const string & title,const string & text);
		virtual bool deleteArticle(int id);
		// map<unsigned int, Article>* listArticles(){ return 0;};
		virtual int createNg(const string &); // return id for the new added or existing newsgroup
		virtual bool deleteNg(unsigned int);
		// Newsgroup* deleteNg(unsigned int){ return 0;};
		ng_map_type& listNg();
		virtual void debugPrint() const;
		virtual int findNg(const string &); // return 0 if no match
		virtual int findNg(int ng_id);



private:
	//Some datastructure... think a map is the best...
	ng_map_type newsgroups;
	art_map_type articles;
	static int lastNgId,lastArtId; // static variable which remember the last id of newsgroup

};

} /* namespace server_client */
#endif /* STORAGEMEMORY_H_ */
