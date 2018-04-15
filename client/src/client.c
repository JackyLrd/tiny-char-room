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
#include "../include/client.h"

struct client_args
{
	int client_fd;
};

void* func(void* args);
void process(int client_fd);

int main()
{
	int client_fd;
	struct sockaddr_in server_addr;
	pthread_t tid;
	struct CLIENTMSG sendMSG;
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

	int len = recv(client_fd, &sendMSG, sizeof(sendMSG), 0);
	if (len > 0)
	{
		if (sendMSG.op == OK) // get ok from the server
		{
			bzero(&sendMSG, sizeof(sendMSG));
			// send client name to the server
			printf("please input client name: ");
			fgets(sendMSG.user_name, CLIENT_NAME_LENGTH, stdin);
			char* find;
			find = strchr(sendMSG.user_name, '\n');
			if (find)
				*find = '\0';
			sendMSG.op = USER;
			send(client_fd, &sendMSG, sizeof(sendMSG), 0);

			// create thread to show the messages
			args = (struct client_args*)malloc(sizeof(struct client_args));
			args->client_fd = client_fd;
			pthread_create(&tid, NULL, func, (void*)args);

			// read the client input
			while (1)
			{
				sendMSG.op = MSG;
				fgets(sendMSG.buf, MSG_LENGTH, stdin);
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
		pthread_cancel(tid);
	}

	close(client_fd);
	return 0;
}

void* func(void* args)
{
	struct client_args* client = (struct client_args*)args;
	process(client->client_fd);
	free(args);
	args = NULL;
	pthread_exit(NULL);
}

void process(int client_fd)
{
	int len;
	struct CLIENTMSG clientMSG;
	// receive message and show them
	while (1)
	{
		bzero(&clientMSG, sizeof(clientMSG));
		len = recv(client_fd, &clientMSG, sizeof(clientMSG), 0);
		if (len > 0)
		{
			if (clientMSG.op == USER)
				printf("user %s is login\n", clientMSG.user_name);
			else if (clientMSG.op == EXIT)
				printf("user %s is logout\n", clientMSG.user_name);
			else if (clientMSG.op == MSG)
				printf("%s: %s\n", clientMSG.user_name, clientMSG.buf);
		}
	}
}