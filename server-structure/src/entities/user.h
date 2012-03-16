/*
 * user.h
 *
 *  Created on: Mar 16, 2012
 *      Author: misteryyy
 */

#ifndef USER_H_
#define USER_H_

#include <string>
using namespace std;
namespace client_server {
class User {

public:
	User();
	virtual ~User();

private:
	int id;
	string name;
};
}
#endif /* USER_H_ */
