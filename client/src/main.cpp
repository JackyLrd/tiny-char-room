#define _GNU_SOURCE 1
#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <poll.h>
#include "../include/client.hpp"

#include <string>

int main()
{
	int client_fd;
	sockaddr_in server_addr;
	CLIENTMSG sendMSG;
	CLIENTMSG clientMSG;

	// create client socket fd
	client_fd = socket(PF_INET, SOCK_STREAM, 0);
	assert(client_fd >= 0);

	// initialize server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.223");

	// connect to server
	int ret = connect(client_fd, (sockaddr*)&server_addr, sizeof(sockaddr));
	assert(ret == 0);

	// create poll fd
	pollfd poll_fds[2];

	// set poll fd
	poll_fds[0].fd = 0;
	poll_fds[0].events = POLLIN;
	poll_fds[0].revents = 0;
	poll_fds[1].fd = client_fd;
	poll_fds[1].events = POLLIN | POLLRDHUP;
	poll_fds[1].revents = 0;

	while (1)
	{
		//printf("0");
		ret = poll(poll_fds, 2, -1);
		if (ret < 0)
		{
			printf("poll failure.\n");
			break;
		}
		if (poll_fds[1].revents & POLLRDHUP)
		{
			printf("lost connection with server.\n");
			break;
		}
		if (poll_fds[1].revents & POLLIN)
		{
			memset(&clientMSG, 0, sizeof(clientMSG));
			int len = recv(client_fd, &clientMSG, sizeof(clientMSG), 0);
			if (clientMSG.op == OK)
			{
				printf("please input client name: \n");
				fgets(sendMSG.user_name, CLIENT_NAME_LENGTH, stdin);
				char* find;
				find = strchr(sendMSG.user_name, '\n');
				if (find)
					*find = '\0';
				sendMSG.op = USER;
				send(client_fd, &sendMSG, sizeof(sendMSG), 0);
			}
			else if (clientMSG.op == USER)
				printf("user %s is login.\n", clientMSG.user_name);
			else if (clientMSG.op == EXIT)
				printf("user %s is logout.\n", clientMSG.user_name);
			else if (clientMSG.op == MSG)
				printf("%s: %s\n", clientMSG.user_name, clientMSG.buf);
		}
		if (poll_fds[0].revents & POLLIN)
		{
			sendMSG.op = MSG;
			read(poll_fds[0].fd, sendMSG.buf, MSG_LENGTH);
			char* find;
			find = strchr(sendMSG.buf, '\n');
			if (find)
			*find = '\0';
			// "bye" is the quit code
			if (std::string(sendMSG.buf) == std::string("bye"))
			{
				sendMSG.op = EXIT;
				send(client_fd, &sendMSG, sizeof(sendMSG), 0);
				break;
			}
			send(client_fd, &sendMSG, sizeof(sendMSG), 0);
		}
	}

	close(client_fd);
	return 0;
}