/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 22:04:57 
 * @Last Modified by:   JackyLrd 
 * @Last Modified time: 2018-04-18 22:04:57 
 */
#include <netinet/in.h>
#include <cstring>

const int MSG_LENGTH = 1024;
const int CLIENT_NAME_LENGTH = 256;

const unsigned int INIT = 0;
const unsigned int OK = 1;
const unsigned int EXIT = 2;
const unsigned int MSG = 3;
const unsigned int USER = 4;

class CLIENTMSG
{
	public:
		CLIENTMSG();
		unsigned int op;
		char user_name[CLIENT_NAME_LENGTH];
		char buf[MSG_LENGTH];
};

class CLIENT
{
	public:
		CLIENT();
		int sockfd;
		int port;
		char user_name[CLIENT_NAME_LENGTH];
		sockaddr_in client_addr;
		CLIENT* next_client;
		CLIENT* pre_client;
};