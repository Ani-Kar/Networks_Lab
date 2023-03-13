#ifndef MYSOCKET_HPP
#define MYSOCKET_HPP

#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define SOCK_MyTCP 100
struct MySocket{
    int sockfd;
    pthread_t R,S;
    char **SendMessage, **ReceiveMessage;
};

struct MySocket my_socket(int domain, int type, int protocol);

int my_bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

int my_listen(int sockfd, int backlog);

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int my_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags);

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

int my_close(int fd);

void Recieve_Thread(char **ReceiveMessage);

void Send_Thread(char **SendMessage);

#endif


