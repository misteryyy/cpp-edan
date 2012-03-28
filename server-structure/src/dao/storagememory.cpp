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
#include <exception>


namespace client_server{
using namespace std;

/*
 * Inicialization of counter for id
 */
int StorageMemory::lastNgId = 1;
int StorageMemory::lastArtId = 1;


// NG SECTION

/*
 * Creates new Newsgroup
 * @return Newsgroup& ; if NG exists throw exception
 * @throws newsgroup_already_exists
 */
void StorageMemory::createNg(const string & name) throw (newsgroup_already_exists){
	// newsgroup exist, return id to the existing one
	if(findNg(name)!= 0) throw newsgroup_already_exists();
	newsgroups.insert(ng_pair(lastNgId,Newsgroup(lastNgId,name)));
	lastNgId++; // incrementing id
}


/**
 * Deletes newsgroups and all articles in it
 * @returns bool If operation is successful
 * @throws newsgroup_doesnt_exists
 */
void StorageMemory::deleteNg(unsigned int index)  throw(newsgroup_doesnt_exists){

	//Checking index!
	for (map<unsigned, Newsgroup>::iterator it = newsgroups.begin(); it != newsgroups.end(); it++){
		cout << "Index: " << it->first << endl;
		cout << "Ng Name: " << it->second.getName() << endl;;
	}

	try{
	Newsgroup& n  = newsgroups.at(index); // throw exception if not found

	// get all articles in NG
	set<unsigned> articles_ng = n.getArticlesId();
	set<unsigned>::iterator it = articles_ng.begin();

		// cleaning articles list
		for (; it != articles_ng.end(); it++){
			articles.erase(*it);
		}

	// erase newsgroup
	newsgroups.erase(index);

	}catch (exception& e) {
		throw newsgroup_doesnt_exists();
	}
}

/**
* @returns reference to newsgroup list
*/
ng_map_type& StorageMemory::listNg(){
	return newsgroups;
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

/**
 * @returns integer ID number of newsgroup
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

//**********************************
//
// ARTICLES METHODS
//
//***********************************


/*
* Creates new Article in the memory
* @return int ID of the new added article
* @throws newsgroup_doesnt_exists
*/
int StorageMemory::createArticle(int ng_id, const string & title ,const string & author,const string & text) throw (newsgroup_doesnt_exists){
	if ( findNg(ng_id) == 0) throw newsgroup_doesnt_exists(); //

	// adds new article
	articles.insert(art_pair(lastArtId,Article(lastArtId,ng_id,title,author,text)));
	// add article id to newsgroup for faster searching
	newsgroups.find(ng_id)->second.addArticleId(lastArtId);
	return lastArtId++;
}

/**
* Return new list with articles in newsgroups
* @returns list
* @throws newsgroup_doesnt_exists
*/
art_map_type StorageMemory::listArticlesInNg(signed ng_id) throw(newsgroup_doesnt_exists)
{
	// newsgroup doesn't exists
	if(findNg(ng_id) == 0) throw newsgroup_doesnt_exists();

	// return list of articles
	art_map_type articles_in_ng;

	// articles in the newsgroup
	set<unsigned> articles_ng = newsgroups.find(ng_id)->second.getArticlesId();
	set<unsigned>::iterator it = articles_ng.begin();

	for (; it != articles_ng.end(); it++){
			// selecting id and article from the articles list
			unsigned id = articles.find(*it)->first;
			Article a= articles.find(*it)->second;

			// saving to the filtered articles list
			articles_in_ng.insert(art_pair(id,a));
	}

	return articles_in_ng;

};


/*
 * Deletes article by id and ng_id
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
void StorageMemory::deleteArticle(int id, int ng_id)  throw(newsgroup_doesnt_exists,article_doesnt_exists){

	// newsgroup doesn't exists
	if(findNg(ng_id) == 0) throw newsgroup_doesnt_exists();
	if(testArticleId(id,ng_id) == 0 ) throw article_doesnt_exists();

	// first delete id in the newsgroup
	int id_ng =articles.find(id)->second.getNgId();
	newsgroups.find(id_ng)->second.deleteArticleId(id);
	// then delete article from the map
	articles.erase(id);
}

// Are we using this somewhere???
art_map_type& StorageMemory::listArticles(){
		return articles;
}

/*
 * Deletes article by id and ng_id
 * @return Article& reference to the Article Entity
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
Article& StorageMemory::findArticle(int id,int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists) {
	if(findNg(ng_id) == 0) throw newsgroup_doesnt_exists();
	if(testArticleId(id,ng_id) == 0 ) throw article_doesnt_exists();

	return articles.find(id)->second;
}


/*
 * Return 0 if the article id is not found
 */
 int StorageMemory::testArticleId(int id, int ng_id){

	 try{
		// if article doesn't exist, it will throw exception
	 	Article& a = articles.at(id);
	 	if(a.getNgId() != ng_id) return false;
	 	// TODO maybe check set of articles in newsgroup for sure? But is should be consistent
	 	return id;
	 	}catch (exception& e) {
	 		return false;
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
