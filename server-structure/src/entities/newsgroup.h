/*
 * newsgroup.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef NEWSGROUP_H_
#define NEWSGROUP_H_

#include <iostream>
#include <algorithm>
#include <set>
#include <iterator>



namespace client_server {

class Newsgroup {
	//output and input stream, useful for saving data to file in format
	friend std::istream& operator>>(std::istream& is, Newsgroup& d);
	friend std::ostream& operator<<(std::ostream& os, const Newsgroup& d){
		os << d.id << "," << d.name << ",";
		// displaying articles in the group
		os << "article_ids:";
		ostream_iterator<signed> out_it (os,", ");
		copy ( d.articles.begin(),  d.articles.end(), out_it );
		return os;
	};

public:
	Newsgroup(int i, const string & n){ name=n, id=i;};
	~Newsgroup(){};
	//overloaded operators
	bool operator!=(const Newsgroup& ngr) const {
		if (name != ngr.name || id != ngr.id ) return true;
		return false;
	};

	bool operator==(const Newsgroup& ngr) const{
		if (name == ngr.name && id == ngr.id ) return true;
		return false;
	};;

	string const & getName()const {return name;} ;
	int getId(){return id;};
	void setName(const string & in_name){ name=in_name;}
   	void addArticleId(unsigned id){
   		articles.insert(id);
   	}
   	set<unsigned>& getArticlesId(){
   		return articles;
   	}

   	bool deleteArticleId(unsigned id){
   		return articles.erase(id);
   	}


private:
	 string name;
	 int id;
	 set<unsigned> articles;  // id keys for articles

};

}
#endif /* NEWSGROUP_H_ */
