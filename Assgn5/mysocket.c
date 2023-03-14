#include "mysocket.h"

struct MySocket_Struct mySocket;
pthread_mutex_t recieveBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sendBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recieveUpdated = PTHREAD_COND_INITIALIZER;
pthread_cond_t SendUpdated = PTHREAD_COND_INITIALIZER;

int my_socket(int domain, int type, int protocol)
{

    int sockfd;
    if (type != SOCK_MyTCP)
    {
        perror("Socket Type Not Supported");
        return -1;
    }
    mySocket.sockFDServer = socket(domain, SOCK_STREAM, protocol);

    mySocket.sockFDClient = -1;
    mySocket.isServer = 0;

    mySocket.ReceiveMessage = (char **)malloc(sizeof(char *) * 10);
    mySocket.SendMessage = (char **)malloc(sizeof(char *) * 10);

    if (mySocket.sockFDServer < 0)
    {
        perror("Error Creating Socket");
        return -1;
    }

    pthread_create(&mySocket.R, NULL, Recieve_Thread, mySocket.ReceiveMessage);
    pthread_create(&mySocket.S, NULL, Send_Thread, mySocket.SendMessage);

    return mySocket.sockFDServer;
}

int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) < 0)
    {
        perror("Unable To bind Local Address");
        return -1;
    }
    return 0;
}

int my_listen(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) < 0)
    {
        perror("Unable to Listen");
        return -1;
    }
    return 0;
}

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    if (mySocket.sockFDServer != sockfd)
    {
        perror("Socket Error");
        exit(1);
    }
    mySocket.sockFDClient = accept(sockfd, addr, addrlen);
    if (mySocket.sockFDClient < 0)
    {
        perror("Error in Accepting");
        return -1;
    }
    mySocket.isServer = 1;
    return mySocket.sockFDClient;
}

int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (mySocket.sockFDServer != sockfd)
    {
        perror("Socket Error");
        exit(1);
    }

    mySocket.isServer = 0;
    mySocket.sockFDClient = mySocket.sockFDServer;
    mySocket.sockFDServer = -1;

    if (connect(sockfd, addr, addrlen) < 0)
    {
        perror("Unable to Connect");
        return -1;
    }
    return 0;
}

int my_close(int fd)
{

    if (mySocket.sockFDServer == fd)
    {
        mySocket.sockFDClient = -1;
    }
    else if (mySocket.sockFDClient == fd)
    {
        mySocket.sockFDClient = -1;
    }
    else
    {
        perror("Socket Error");
        exit(1);
    }

    if (close(fd) < 0)
    {
        perror("Unable to Close");
        return -1;
    }
    return 0;
}

ssize_t my_send(int sockfd, const void *buf, size_t len, int flags)
{
    char messageSize[50];
    sprintf(messageSize, "%d", len);
    char *newMessage = (char *)malloc(len + 8 * (sizeof(char)) + strlen(messageSize));
    int i = 0;
    for (; i < strlen(messageSize); i++)
    {
        newMessage[i] = messageSize[i];
    }
    for (; i < strlen(messageSize) + 4; i++)
    {
        newMessage[i] = '#';
    }
    char *message = buf;
    for (; i < strlen(messageSize) + 4 + len; i++)
    {
        newMessage[i] = message[i];
    }
    for (; i < strlen(messageSize) + 4 + len + 4; i++)
    {
        newMessage[i] = '#';
    }
    pthread_mutex_lock(&sendBuffer);
    while (mySocket.receiveTableSize == MAX_TABLE_ENTRIES)
    {
        pthread_cond_wait(&sendBuffer, &SendUpdated);
    }
    mySocket.SendMessage[mySocket.sendTableSize++] = newMessage;
    pthread_mutex_unlock(&sendBuffer);
    return len;
}

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags)
{
    char *message;
    char size_char[50];
    pthread_mutex_lock(&recieveBuffer);
    while (mySocket.receiveTableSize < 1)
    {
        pthread_cond_wait(&recieveBuffer, &recieveUpdated);
    }
    message = mySocket.ReceiveMessage[0];
    for (int i = 1; i < mySocket.receiveTableSize; i++)
    {
        mySocket.ReceiveMessage[i - 1] = mySocket.ReceiveMessage[i];
    }
    dealloc(mySocket.ReceiveMessage[mySocket.receiveTableSize - 1]);
    mySocket.receiveTableSize--;
    pthread_mutex_unlock(&recieveBuffer);

    char *buffer = (char *)malloc(sizeof(char) * 5050);
    buffer = buf;
    int i = 0;
    while (message[i] != '#')
    {
        size_char[i] = message[i];
        i++;
    }
    size_char[i] = '\0';
    int size = atoi(size_char);
    i = i + 4;
    for (; i < min(len, size); i++)
    {
        buffer[i] = message[i + 4 + strlen(size_char)];
    }
    return min(len, size);
}

void *Recieve_Thread(void *params)
{
    ;
}

void *Send_Thread(void *params)
{
    while (1)
    {
        sleep(5);
        pthread_mutex_lock(&sendBuffer);

        pthread_mutex_unlock(&sendBuffer);
    }
}