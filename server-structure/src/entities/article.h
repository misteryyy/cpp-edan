/*
 * article.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef ARTICLE_H_
#define ARTICLE_H_

#include <iostream>

using namespace std;
namespace client_server {


class Article {

	//output and input stream, useful for saving data to file in format
	friend std::istream& operator>>(std::istream& is, Article& a);

	friend std::ostream& operator<<(std::ostream& os, const Article & a){
		os << a.id << "," << a.ng_id << "," << a.author <<","<< a.name << "," << a.text << ";";
		return os;
	};

public:
	Article (int idp, int ng_idp, const string & namep,const string & auth,const string & textp) {
		id=idp; ng_id=ng_idp; name = namep; text=textp; author=auth;
	};

   ~Article(){};

   //overloaded operators
   	bool operator!=(const Article& ngr) const {
   		if (id != ngr.id ) return true;
   		return false;
   	};

   	bool operator==(const Article& ngr) const{
   		if (id == ngr.id ) return true;
   		return false;
   	};;

   	string const & getName()const {return name;} ;
   	int getId(){return id;};
   	int getNgId(){return ng_id;};
   	void setName(const string & in_name){ name=in_name;}


private:
	int id;
	int ng_id; // foreign key for newsgroup
	string name, text, author;
};

} /* namespace client_server */
#endif /* ARTICLE_H_ */
