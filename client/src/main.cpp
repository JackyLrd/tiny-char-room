#define _GNU_SOURCE 1
#include <cstdio>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string>
#include "../include/client.hpp"

int main()
{
	int client_fd;
	bool server_on = false;
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

	server_on = true;

	// create epoll fd
	int epoll_fd = epoll_create(2);
	assert(epoll_fd != -1);

	// add events
	epoll_event events[1024];
	events[0].events = EPOLLIN;
	events[0].data.fd = 0;
	events[1].events = EPOLLIN | EPOLLRDHUP;
	events[1].data.fd = client_fd;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &events[0]);
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &events[1]);

	while (1)
	{
		ret = epoll_wait(epoll_fd, events, 1024, -1);
		if (ret < 0)
		{
			printf("epoll failure.\n");
			break;
		}
		for (int i = 0; i < ret; ++i)
		{
			int fd = events[i].data.fd;
			// if the fd is stdin
			if (fd == 0)
			{
				sendMSG.op = MSG;
				read(fd, sendMSG.buf, MSG_LENGTH);
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
			// if the fd is client socket
			else
			{
				if (events[i].events & EPOLLRDHUP)
				{
					printf("lost connection with server.\n");
					server_on = false;
					break;
				}
				if (events[i].events & EPOLLIN)
				{
					memset(&clientMSG, '\0', sizeof(clientMSG));
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
			}
		}
		if (!server_on)
			break;
	}

	close(client_fd);
	return 0;
}