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
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include "string.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace client_server {
using namespace std;


 // creates string from int is one digit adds zero; ex: 1,2,3 -> 01 02 03
 string getStringFromInt(int n){
    stringstream out;
     out << n;
     string s = out.str();
     return s;
 }

 // creates int from string
 int getIntFromString(string s){
     stringstream out(s);
     int i;
     out >> i;
     return i;
 }

/**
 * Function which will create vector from text 24-newsgroup name; or 2-article title - author
 * @return vector of all separated words in the string vector
 * idea from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
 */
 vector<string> split(const string &s, char delim){

	stringstream ss(s);
	string item;
	vector<string> parse;

	while(getline(ss, item, delim)) {
	 	          parse.push_back(item);
	 }

	return parse;
 }

StorageDisk::StorageDisk(){
	baseDir = "./storage/";
	// load data to the newsgroups map
	buildNgList();

	//load the lastIDs for NG and Articles
	string settingPath = baseDir + "settings.txt";

	 // reading parameters from the setting file
	 string newsgroupID;
	 string articleID;
	 ifstream in(settingPath.c_str());

	  // Inicialization of counter for id
	 	 lastNgId = 1;
		 lastArtId = 1;
	 if (! in) {
	       cerr << "Could not open data file with settings for last IDs" << endl;
	        exit(1);
	 } else {
	 // reading data for id
	 in >> newsgroupID >> articleID;
	 	 lastNgId = getIntFromString(newsgroupID);
	 	 lastArtId = getIntFromString(articleID);
	 in.close();
	 }

	 cout << "Program started with NG ID " << newsgroupID;
	 cout << " and Article ID" << articleID << endl;

}
/**
 * Destructor which saves data to setting.txt, lastID for articles and newsgroups
 */
StorageDisk::~StorageDisk(){
	cout << "Destructor call: ";
	//load the lastIDs for NG and Articles
	string settingPath = baseDir + "settings.txt";

	ofstream out(settingPath.c_str());

	if (! out) {
		       cerr << "Could not open data file with settings for saving IDs" << endl;
		        exit(1);
	} else {
		 // saving data
		 out << lastNgId << " " << lastArtId;
		 out.close();
		 cout << lastNgId << " and " << lastArtId << " we saved " << endl;
	}



}


void StorageDisk::buildNgList(){
	// loading file structure to the list for manipulating with NG methods
			 newsgroups.clear(); // destroy old data
			  DIR *pDIR;
			  struct dirent *entry;

			  if( (pDIR=opendir(baseDir.c_str())) ){
			                while( (entry = readdir(pDIR) )){

			                if( strcmp(entry->d_name, ".") != 0 &&
			                	strcmp(entry->d_name, "..") != 0  &&
			                	strcmp(entry->d_name, "settings.txt") != 0 ) {

			                	//cout << entry->d_name << "\n"; // d_name is char array, we need string
			                	string dir_name(entry->d_name); // creating of string variable

			                	vector<string> ng_parse = split(dir_name,'-'); // expected size 2 ID and Title

			                	if(ng_parse.size() == 2){
			                		int ng_id = getIntFromString(ng_parse[0]);
			                		Newsgroup ng(ng_id,ng_parse[1]); // new newsgroup
			                		newsgroups.insert(ng_pair(ng_id,ng));
			                	}

			                }

			                }
			                closedir(pDIR);
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

/**
 * @returns integer ID number of newsgroup
 */
int StorageDisk::findNg(const string & name){
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
int StorageDisk::findNg(int ng_id){
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


/*TODO fix this for use with list of articles
 * Return 0 if the article id is not found
 */
 int StorageDisk::testArticleId(int id, int ng_id){

	 try{
		// if article doesn't exist, it will throw exception
	 	Article& a = listArticlesInNg(ng_id).at(id);
	 	if(a.getNgId() != ng_id) return 0;
	 	return id;
	 	}catch (exception& e) {
	 		return 0;
	 	}
 }


// NG SECTION
/*
 * Creates new Newsgroup
 * @return Newsgroup& ; if NG exists throw exception
 * @throws newsgroup_already_exists
 */
void StorageDisk::createNg(const string & name) throw (newsgroup_already_exists){

	// newsgroup exist, return id to the existing one
	if(findNg(name)!= 0) throw newsgroup_already_exists();

	//mkdir return -1 if dir already exists, 0 if new folder created
	cout << "Creating dir in " << baseDir  << lastNgId << "-" << name << endl;
	string dir = baseDir + getStringFromInt(lastNgId).append("-") + name; // creating ng folder name

	if(mkdir(dir.c_str(),0777) != 0) throw newsgroup_already_exists();

	lastNgId++; // incrementing id

	// rebuild newsgroupList
	// TODO possible solution, to add just new id to the map and not call rebuild
	buildNgList();
}

/**
 * Deletes newsgroups and all articles in it
 * @returns bool If operation is successful
 * @throws newsgroup_doesnt_exists
 */
void StorageDisk::deleteNg(unsigned int index)  throw(newsgroup_doesnt_exists){

	// newsgroup exist, return id to the existing one
	if(findNg(index)== 0) throw newsgroup_doesnt_exists();

	string dir = baseDir + getStringFromInt(index) + "-" + newsgroups.find(index)->second.getName();
	//cout << "This dir will be deleted > "<< dir << endl;

	// deleting all articles in this folder
	art_map_type articles_to_delete = listArticlesInNg(index);

	if (articles_to_delete.size() >0){

		art_map_type::iterator it = articles_to_delete.begin();
		for(;it != articles_to_delete.end(); it++){

			//cout << "Article ID will be DELETED > " << it->second.getId() << endl;
			deleteArticle(it->second.getId(),index);
		}

	}

	// directory has to be empty, delete all articles before
	if( rmdir(dir.c_str()) != 0 ) throw newsgroup_doesnt_exists(); // somethind bad happend

	// rebuild newsgroupList
	// TODO possible solution, delete only newsgroup
	buildNgList();

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

	if ( findNg(ng_id) == 0) throw newsgroup_doesnt_exists(); //

	//creating path for this article
	string ng_dir = baseDir + getStringFromInt(ng_id).append("-") + newsgroups.find(ng_id)->second.getName() + "/";
	Article a(lastArtId,ng_id,title,author,text);

	// whole path with file name ex. ./storage/1-NG 1/1-Title of Article-Author name
	string file_path = ng_dir + a.getFileName();
	//cout << "Create file in: " << file_path << endl;

	//creating new article in newsgroup folder
	ofstream outfile (file_path.c_str());
	string proc = ng_dir + a.getFileName();
	outfile << text << std::endl;
	outfile.close();

	return 	lastArtId++;
}

/**
* Return new list with articles in newsgroups
* @returns list
* @throws newsgroup_doesnt_exists
*/
art_map_type StorageDisk::listArticlesInNg(signed ng_id) throw(newsgroup_doesnt_exists)
{
	if ( findNg(ng_id) == 0) throw newsgroup_doesnt_exists(); //

	// getNG name and id, to find folder
	string dir_ng = baseDir + getStringFromInt(ng_id) +"-"+ newsgroups.find(ng_id)->second.getName();
	//cout << " Finding articles in : " << dir_ng << endl;

	// return list of articles
	art_map_type articles_in_ng;
	  	 DIR *pDIR;
	  	 struct dirent *entry;

		  if( (pDIR=opendir(dir_ng.c_str()) ) ){
			  while( (entry = readdir(pDIR) )){

		                if( strcmp(entry->d_name, ".") != 0 &&
		                	strcmp(entry->d_name, "..") != 0 ) {

		                	//cout << entry->d_name << "\n"; // d_name is char array, we need string
		                	string article_name(entry->d_name); // creating of string variable

		                	vector<string> article_parse = split(article_name,'-'); // expected size 2 ID-Title-Author
				             	if(article_parse.size() == 3){ // 3 parameters ID,Title,Author

				             		int article_id = getIntFromString(article_parse[0]);
				             		string article_title = article_parse[1];
				             		string article_author = article_parse[2];

				             		// reading text
				             		string file_path = dir_ng + "/" + article_name;
				             		ifstream infile(file_path.c_str());

				             		string line;
				             		string article_text;
				             		// reading the file content in lines
				             			if (infile.is_open())
				             		     {
				             		       while ( infile.good() )
				             		       {
				             		         getline (infile,line);
				             		         article_text += line; // reading all lines in the file
				             		       }
				             		       infile.close();
				             		     }
				             		     else {
				             		    	 cout << "Unable to open file:" << file_path << endl;
				             		     }

				         Article a(article_id,ng_id,article_title,article_author,article_text);
				        // cout << "This article has :" << a << endl;

				         // saving to the filtered articles list
				         articles_in_ng.insert(art_pair(article_id,a));
				         }

		        }

			}
			closedir(pDIR);
		  }

	return articles_in_ng;

}


/*
 * Deletes article by id and ng_id
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
void StorageDisk::deleteArticle(int id, int ng_id)  throw(newsgroup_doesnt_exists,article_doesnt_exists){

	if ( findNg(ng_id) == 0) throw newsgroup_doesnt_exists(); //
	if(testArticleId(id,ng_id) == 0 ) throw article_doesnt_exists();

	string ng_path = baseDir + getStringFromInt(ng_id) + "-" +newsgroups.find(ng_id)->second.getName();
	string article_path = ng_path + "/" + listArticlesInNg(ng_id).find(id)->second.getFileName();

	//cout << "This article will be deleted" << remove(article_path.c_str()) << endl;
	if(remove(article_path.c_str()) != 0) throw article_doesnt_exists(); // something bad happened with the file :D



}

// TODO DELETE THIS, WE DON=T NEED IT
art_map_type& StorageDisk::listArticles(){
		return articles;
}

/*
 * find article by id and ng_id
 * @return Article& reference to the Article Entity
 * @throws newsgroup_doesnt_exists or article_doesnt_exists
 */
Article& StorageDisk::findArticle(int id,int ng_id) throw(newsgroup_doesnt_exists,article_doesnt_exists) {

	if ( findNg(ng_id) == 0) throw newsgroup_doesnt_exists(); //

	if(testArticleId(id,ng_id) == 0 ) throw article_doesnt_exists();

	cout << "TRYING TO GET A ARTICLE" << endl;
	return listArticlesInNg(ng_id).find(id)->second;
}


/* ----------------------------------
 * DEBUGGING STUFF
 * ==================================
 */

/*
 * Function for displaying pair
 */
void displayPairDisk(ng_pair r) {
     cout << r.first << "; object{" << r.second  << " }" <<endl;
 }


/*
 * Function for displaying map
 *
 */
void displayMapDisk(ng_map_type  m) {
     for_each(m.begin(), m.end(), displayPairDisk);
     cout << "Size:" << m.size() << endl;
}

/*
 * Function for debuging map, uses displayMap and displayPair
 */
void StorageDisk::debugPrint() const{
	cout << "--Begin of DEBUG of DISK--" << endl;
	cout << "--NEWSGROUP map--" << endl;
	displayMapDisk(newsgroups);
	cout << "--================--" << endl;
};

} /* namespace server_client */
