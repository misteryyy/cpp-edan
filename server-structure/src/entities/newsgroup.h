/*
 * newsgroup.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef NEWSGROUP_H_
#define NEWSGROUP_H_

using namespace std;

#include <string>

namespace client_server {

class Newsgroup {
	public:
		Newsgroup();
		virtual ~Newsgroup();
	private:
		string name;
		unsigned int id;
		map<unsigned int, Article> articles;
	};
}
#endif /* NEWSGROUP_H_ */
