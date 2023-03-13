#include "mysocket.h"

struct MySocket my_socket(int domain, int type, int protocol)
{
    struct MySocket mySocket;
    int sockfd;
    if (type != SOCK_MyTCP)
    {
        perror("Socket Type Not Supported");
        exit(1);
    }
    mySocket.sockfd = socket(domain, SOCK_STREAM, protocol);
    if (mySocket.sockfd < 0)
    {
        perror("Error Creating Socket");
        exit(1);
    }
    pthread_create(&mySocket.R, NULL, Recieve_Thread, mySocket.ReceiveMessage);
    pthread_create(&mySocket.S, NULL, Send_Thread, mySocket.SendMessage);

    return mySocket;
}

int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) < 0)
    {
        perror("Unable To bind Local Address");
        exit(1);
    }
}

int my_listen(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) < 0)
    {
        perror("Unable to Listen");
        exit(1);
    }
}

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    if (accept(sockfd, addr, addrlen) < 0)
    {
        perror("Error in Accepting");
        exit(1);
    }
}

int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (connect(sockfd, addr, addrlen) < 0)
    {
        perror("Unable to Connect");
        exit(1);
    }
}

int my_close(int fd)
{
    if (close(fd) < 0)
    {
        perror("Unable to Close");
        exit(1);
    }
}

ssize_t my_send(int sockfd, const void *buf, size_t len, int flags){
    ;
}

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags){
    ;
}

void* Recieve_Thread(void* params){
    ;
}

void* Send_Thread(void* params){
    ;
}