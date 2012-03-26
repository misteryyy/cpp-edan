/*
 * storagedisk.cpp
 *
 *  Created on: 20 mar 2012
 *      Author: Chris
 *
 *      Implements StorageInterface
 */

#include "storagedisk.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <exception>

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include "string.h"
#include <string>
#include <sstream>



namespace client_server {
using namespace std;
/*
 * Inicialization of counter for id
 */
int StorageDisk::lastNgId = 1;
int StorageDisk::lastArtId = 1;


StorageDisk::StorageDisk(){
	cout << "Constructor" << endl;
	baseDir = "./storage/";

	string NGDir = "";
	mkdir(baseDir.c_str(),0777);

	//load the lastIDs for NG and Articles
	//string settingPath = baseDir + "settings.txt";

	//ofstream settingFile(settingPath.c_str());
	//settingFile << "Writing this to a file.\n";
	//settingFile.close();


	  DIR *pDIR;
	  struct dirent *entry;
	 // struct stat stat_base;

	  if( pDIR=opendir(baseDir.c_str()) ){
	                while(entry = readdir(pDIR)){

	                if( strcmp(entry->d_name, ".") != 0 &&
	                	strcmp(entry->d_name, "..") != 0  &&
	                	strcmp(entry->d_name, "settings.txt") != 0 ) {
	                	cout << entry->d_name << "\n";
	                }

	                }
	                closedir(pDIR);
	  }
}

void StorageDisk::listFile(){
        DIR *pDIR;
        struct dirent *entry;
        if( pDIR=opendir(baseDir.c_str()) ){

        		while(entry = readdir(pDIR)){

        				if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
        				cout << entry->d_name << "\n";
        		}
                closedir(pDIR);
        }
}



StorageDisk::~StorageDisk(){
	cout << "Destructor" << endl;
}

// NG SECTION
/*
 * Creates new Newsgroup
 * @return Newsgroup& ; if NG exists throw exception
 * @throws newsgroup_already_exists
 */
void StorageDisk::createNg(const string & name) throw (newsgroup_already_exists){
	lastNgId++; // incrementing id
}

/**
 * Deletes newsgroups and all articles in it
 * @returns bool If operation is successful
 * @throws newsgroup_doesnt_exists
 */
void StorageDisk::deleteNg(unsigned int index)  throw(newsgroup_doesnt_exists){


}

/**
* @returns reference to newsgroup list
*/
ng_map_type& StorageDisk::listNg(){
	return newsgroups;
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
int StorageDisk::createArticle(int ng_id, const string & title ,const string & author,const string & text) throw (newsgroup_doesnt_exists){


	return 1;
}

/**
* Return new list with articles in newsgroups
* @returns list
* @throws newsgroup_doesnt_exists
*/
art_map_type StorageDisk::listArticlesInNg(signed ng_id) throw(newsgroup_doesnt_exists)
{

	return articles;

};


/*
 * Deletes article by id and ng_id
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
void StorageDisk::deleteArticle(int id, int ng_id)  throw(newsgroup_doesnt_exists,article_doesnt_exists){



}

art_map_type& StorageDisk::listArticles(){
		return articles;
}

/*
 * Deletes article by id and ng_id
 * @return Article& reference to the Article Entity
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
Article& StorageDisk::findArticle(int id,int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists) {

	return articles.find(id)->second;
}





/*
 * Function for debuging map, uses displayMap and displayPair
 */
void StorageDisk::debugPrint() const{
	cout << "--Begin of DEBUG of DISK--" << endl;
	cout << "--NEWSGROUP map--" << endl;
	//displayMap(newsgroups);
	cout << "--================--" << endl;

	cout << "--ARTICLES map--" << endl;
	//displayMapArt(articles);
	cout << "--End of DEBUG--" << endl;



};

} /* namespace server_client */
