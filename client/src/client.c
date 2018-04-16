#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <fcntl.h> 
#include "../include/client.h"

int main()
{
	int client_fd;
	struct sockaddr_in server_addr;
	pthread_t tid;
	struct CLIENTMSG sendMSG;
	bzero(&sendMSG, sizeof(sendMSG));
	struct CLIENTMSG clientMSG;
	struct client_args* args;

	// create client socket fd
	client_fd = socket(PF_INET, SOCK_STREAM, 0);
	assert(client_fd >= 0);

	// initialize server address
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.223");

	// connect to server
	int ret = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	assert(ret == 0);

	// create poll fd
	struct pollfd poll_fds[2];

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
			bzero(&clientMSG, sizeof(clientMSG));
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
			if (!strncasecmp(sendMSG.buf, "bye", 3))
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