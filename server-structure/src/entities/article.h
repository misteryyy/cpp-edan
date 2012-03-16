/*
 * article.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef ARTICLE_H_
#define ARTICLE_H_

#include "user.h"

using namespace std;
namespace client_server {

class Article {
public:
	Article();
   ~Article();

private:
	int id;
	User * user;
	string name, text;
};

} /* namespace client_server */
#endif /* ARTICLE_H_ */
