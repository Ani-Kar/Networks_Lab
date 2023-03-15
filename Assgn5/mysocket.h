#ifndef MYSOCKET_HPP
#define MYSOCKET_HPP

#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<poll.h>

#define SOCK_MyTCP 100
#define MAX_TABLE_ENTRIES 10
#define MAXSIZE 1000
#define min(a,b) (a>b?b:a)

struct MySOcket_Struct;
extern pthread_mutex_t recieveBuffer;
extern pthread_mutex_t sendBuffer;
extern pthread_cond_t recieveUpdated;
extern pthread_cond_t SendUpdated;
extern struct MySocket_Struct mySocket;

struct MySocket_Struct{
    int sockFDServer;
    int sockFDClient;
    int isServer;
    pthread_t R,S;
    char **SendMessage, **ReceiveMessage;
    int sendTableSize;
    int receiveTableSize;
};

int my_socket(int domain, int type, int protocol);

int my_bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

int my_listen(int sockfd, int backlog);

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int my_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags);

ssize_t my_send(int sockfd, const void *buf, size_t len, int flags);

int my_close(int fd);

void* Recieve_Thread(void* params);

void* Send_Thread(void* params);

int get_size(char * message);

char *extractMessage(char *message, int currSize);

#endif


