#include <netinet/in.h>

#define MSG_LENGTH 1024
#define CLIENT_NAME_LENGTH 256

const unsigned int INIT = 0;
const unsigned int OK = 1;
const unsigned int EXIT = 2;
const unsigned int MSG = 3;
const unsigned int USER = 4;

struct CLIENTMSG
{
	unsigned int op;
	char user_name[CLIENT_NAME_LENGTH];
	char buf[MSG_LENGTH];
};

struct CLIENT
{
	int sockfd;
	int port;
	char user_name[CLIENT_NAME_LENGTH];
	struct sockaddr_in client_addr;
	struct CLIENT *next_client;
	struct CLIENT *pre_client;
};