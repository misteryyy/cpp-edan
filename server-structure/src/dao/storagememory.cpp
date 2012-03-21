/*
 * storagememory.cpp
 *
 *  Created on: 20 mar 2012
 *      Author: Josef
 */

#include "storagememory.h"
#include <iostream>
#include <algorithm>
#include <utility>

namespace client_server{
using namespace std;

/*
 * Inicialization of counter for id
 */
int StorageMemory::lastNgId = 1;
int StorageMemory::lastArtId = 1;


/*
 * Creates new Newsgroup
 * @return id of the last newsgroup
 */
int StorageMemory::createNg(const string & name){
	// newsgroup exist, return id to the existing one
	if(findNg(name)!= 0) {
		// return id to existing newsgroup
		return findNg(name);
	} else {
	newsgroups.insert(ng_pair(lastNgId,Newsgroup(lastNgId,name)));
	return lastNgId++;

	}
}

/*
 * Deletes article by id
 */
bool StorageMemory::deleteArticle(int id){
	if(findArticle(id) == 0 ){

		return false;
	}
	else {
		// first delete id in the newsgroup
		int id_ng =articles.find(id)->second.getNgId();
		newsgroups.find(id_ng)->second.deleteArticleId(id);
		// after delete article from the map
		if (articles.erase(id)){
			return true;
		};
	}


}

/*
 * Deletes newsgroups and all articles in it
 * TODO implement deleting of all articles?
 */
bool StorageMemory::deleteNg(unsigned int index){
	if( newsgroups.erase(index)) {
		return true;
	}
	else {
		return false;
	}
}
  //used for finding in the map according to string value
  struct first_equal : public std::binary_function< ng_pair, string, bool> {
	  bool operator() (const ng_pair& p, const string & s) const {
		  	  if(p.second.getName() == s){
		  		  return true;
		  	  }
		  	  else {
		  		  return false;
		  	  }
     }
 };

/*
 * Return list of Newsgroups
 */
ng_map_type& StorageMemory::listNg(){
	return newsgroups;
}

/*
 * 0 if the newsgroup doesn't exists
 */
int StorageMemory::findNg(const string & name){
	map<unsigned int, Newsgroup>::iterator result;
	// using structor for comparing string values in the map
	result = find_if(newsgroups.begin(), newsgroups.end(), bind2nd(first_equal(), name));
	if (result != newsgroups.end()) {
	    return result->second.getId();
	} else {
		return 0;
	}
}

/*
 * Find newsgroup by id
 * @return 0 if no match
 */
int StorageMemory::findNg(int ng_id){
	if(ng_id > 0){
			ng_map_type::iterator result =  newsgroups.find(ng_id);

			// no match, return 0
			if(result == newsgroups.end()){
				return 0;
			} else {
				return result->second.getId();
			}

	// negative ng_id, return 0
	}else {
		return 0;
	}
}

	art_map_type& StorageMemory::listArticlesInNg(signed ng_id){

		art_map_type articles_in_ng;

		if(findNg(ng_id)){
			// articles in the newsgroup
			set<unsigned> articles_ng = newsgroups.find(ng_id)->second.getArticlesId();
			set<unsigned>::iterator it = articles_ng.begin();
			for (it; it != articles_ng.end(); it++){

				cout << "Article id " << *it << " is in NG " << ng_id << endl;

				// selecting id and article
				unsigned id = articles.find(*it)->first;
				Article a= articles.find(*it)->second;

				// saving articles for current ng
				articles_in_ng.insert(art_pair(id,a));
			}
		}
		return articles_in_ng;


	};

/*
* Creates new Article in the memory
*/
int StorageMemory::createArticle(int ng_id, const string & author ,const string & title,const string & text){

	if ( findNg(ng_id) != 0){
		// adds new article
		articles.insert(art_pair(lastArtId,Article(1,ng_id, author, title,text)));
		// add article id to newsgroup for faster searching
		newsgroups.find(ng_id)->second.addArticleId(lastArtId);
	}
	return lastArtId++;
}

/*
 * Return 0 if the article id is not found
 */
 int StorageMemory::findArticle(int id){

	 	 if(id > 0){
				art_map_type::iterator result =  articles.find(id);
				// no match, return 0
				if(result == articles.end()){
					return 0;
				} else {
					return result->second.getId();
				}
		// negative id, return 0
		}else {
			return 0;
		}
 }


/* ----------------------------------
 * DEBUGGING STUFF
 * ==================================
 */

/*
 * Function for displaying pair
 */
void displayPair(ng_pair r) {
     cout << r.first << "; object{" << r.second  << " }" <<endl;
 }

void displayPairArt(art_pair r) {
     cout << r.first << "; object{" << r.second  << " }" <<endl;
 }

/*
 * Function for displaying map
 *
 */
void displayMap(ng_map_type  m) {
     for_each(m.begin(), m.end(), displayPair);
     cout << "Size:" << m.size() << endl;
}

void displayMapArt(art_map_type  m) {
     for_each(m.begin(), m.end(), displayPairArt);
     cout << "Size:" << m.size() << endl;
}

/*
 * Function for debuging map, uses displayMap and displayPair
 */
void StorageMemory::debugPrint() const{
	cout << "--Begin of DEBUG--" << endl;
	cout << "--NEWSGROUP map--" << endl;
	displayMap(newsgroups);
	cout << "--================--" << endl;

	cout << "--ARTICLES map--" << endl;
	displayMapArt(articles);
	cout << "--End of DEBUG--" << endl;



};



} /* namespace server_client */
