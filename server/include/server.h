#include <netinet/in.h>

#define MSG_LENGTH 1024
#define CLIENT_NAME_LENGTH 256

#define INIT 0
#define OK 1
#define EXIT 2
#define MSG 3
#define USER 4

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