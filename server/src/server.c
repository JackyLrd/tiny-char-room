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
#include "../include/server.h"

const unsigned int SERVER_PORT = 12345;

void *func(void *args);
void process(struct CLIENT* client);

struct CLIENT head;

int main()
{
	pthread_t tid;
	int err;
	// initialize list head;
	head.sockfd = -1;
	head.port = 0;
	bzero(&head.client_addr, sizeof(head.client_addr));
	memset(head.user_name, '\0', CLIENT_NAME_LENGTH);
	head.next_client = NULL;
	head.pre_client = NULL;

	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;

	// create server socket fd
	server_fd = socket(PF_INET, SOCK_STREAM, 0);
	assert(server_fd >= 0);

	// initialize server ip and port
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.223");

	// bind server socket fd with server address
	int reuse = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	int ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	assert(ret != -1);

	// listen
	ret = listen(server_fd, 0);
	assert(ret != -1);

	// main thread waiting for clients to connect
	while (1)
	{
		socklen_t CLIENT_LENGTH = sizeof(client_addr); 
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &CLIENT_LENGTH);
		assert(client_fd > 0);
		printf("client_fd: %d\n", client_fd);

		// initialize new client
		struct CLIENT* new_client = (struct CLIENT*)malloc(sizeof(struct CLIENT));
		new_client->sockfd = client_fd;
		new_client->port = 12345;
		memset(new_client->user_name, '\0', CLIENT_NAME_LENGTH);
		new_client->client_addr = client_addr;

		// insert new clint into client list
		new_client->pre_client = &head;
		new_client->next_client = head.next_client;
		if (head.next_client)
			head.next_client->pre_client = new_client;
		head.next_client = new_client;

		// create new thread to process new client
		err = pthread_create(&tid, NULL, func, (void*)new_client);
		if (!err)
			printf("thread created\n");
	}

	close(server_fd);
	printf("server closed\n");
	return 0;
}

void* func(void *args)
{
	struct CLIENT* client = (struct CLIENT*)args;
	process(client);
	client->pre_client->next_client = client->next_client;
	if (client->next_client)
		client->next_client->pre_client = client->pre_client;
	free(client);
	client = NULL;
	pthread_exit(NULL);
}

void process(struct CLIENT* client)
{
	int len;
	struct CLIENTMSG sendMSG;
	struct CLIENTMSG recvMSG;

	sendMSG.op = OK;
	send(client->sockfd, &sendMSG, sizeof(sendMSG), 0);

	while (1)
	{
		bzero(&sendMSG, sizeof(sendMSG));
		bzero(&recvMSG, sizeof(recvMSG));

		len = recv(client->sockfd, &recvMSG, sizeof(recvMSG), 0);
		if (len == 0)
		{
			sendMSG.op = EXIT;
			struct CLIENT* temp = head.next_client;
			while (temp != NULL)
			{
				if (strcmp(temp->user_name, recvMSG.user_name))
					send(temp->sockfd, &sendMSG, sizeof(sendMSG), 0);
				temp = temp->next_client;
			}
		}
		if (len > 0)
		{
			if (recvMSG.op == USER)
			{
				sendMSG.op = USER;
				memcpy(client->user_name, recvMSG.user_name, strlen(recvMSG.user_name));
				printf("user %s login from %s:%d\n", client->user_name, inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
			}
			if (recvMSG.op == EXIT)
			{
				sendMSG.op = EXIT;
				memcpy(sendMSG.user_name, client->user_name, strlen(recvMSG.user_name));
				struct CLIENT *temp = head.next_client;
				while (temp != NULL)
				{
					send(temp->sockfd, &sendMSG, sizeof(sendMSG), 0);
					temp = temp->next_client;
				}
				break;
			}
			if (recvMSG.op == MSG)
			{
				printf("%s: %s\n", recvMSG.user_name, recvMSG.buf);
				sendMSG.op = MSG;
			}
			memcpy(sendMSG.user_name, recvMSG.user_name, strlen(recvMSG.user_name));
			memcpy(sendMSG.buf, recvMSG.buf, strlen(recvMSG.buf));
			struct CLIENT *temp = head.next_client;
			while (temp != NULL)
			{
				if (strcmp(temp->user_name, sendMSG.user_name))
					send(temp->sockfd, &sendMSG, sizeof(sendMSG), 0);
				temp = temp->next_client;
			}
		}
	}
}

//192.168.1.223:12345