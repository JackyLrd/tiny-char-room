#include "../include/server.hpp"

CLIENTMSG::CLIENTMSG(): op(0)
{
	memset(user_name, '\0', CLIENT_NAME_LENGTH);
	memset(buf, '\0', MSG_LENGTH);
}

CLIENT::CLIENT(): sockfd(-1), port(0)
{
	memset(user_name, '\0', CLIENT_NAME_LENGTH);
	memset(&client_addr, 0, sizeof(sockaddr_in));
	next_client = NULL;
	pre_client = NULL;
}