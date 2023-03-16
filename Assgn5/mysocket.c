#include "mysocket.h"

struct MySocket_Struct mySocket;
pthread_mutex_t recieveBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sendBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recieveUpdated = PTHREAD_COND_INITIALIZER;
pthread_cond_t SendUpdated = PTHREAD_COND_INITIALIZER;

int get_size(char *message)
{
    char size_char[50];
    int i = 0;
    while (message[i] != '#')
    {
        size_char[i++] = message[i];
    }
    size_char[i] = '\0';
    return atoi(size_char) + strlen(size_char) + 8;
}
void print_table(char **table, int size)
{
    printf("Table Size => %d\n", size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < get_size(table[i]); j++)
        {
            printf("%c", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
int my_socket(int domain, int type, int protocol)
{
    int sockfd;
    if (type != SOCK_MyTCP)
    {
        perror("Socket Type Not Supported");
        return -1;
    }

    mySocket.sockFDServer = socket(domain, SOCK_STREAM, protocol);

    mySocket.sockFDClient = 0;
    mySocket.isServer = 0;
    mySocket.sendTableSize = 0;
    mySocket.receiveTableSize = 0;
    mySocket.RShouldTerminate = 0;
    mySocket.SShouldTerminate = 0;
    mySocket.RshouldWait = 1;
    mySocket.SshouldWait = 1;

    mySocket.ReceiveMessage = (char **)malloc(sizeof(char *) * 10);
    for(int i=0;i<10;i++)
        mySocket.ReceiveMessage[i] = (char *)malloc(sizeof(char) * (MESSAGE_MAX_SIZE+12));
    mySocket.SendMessage = (char **)malloc(sizeof(char *) * 10);
    for(int i=0;i<10;i++)
        mySocket.SendMessage[i] = (char *)malloc(sizeof(char) * (MESSAGE_MAX_SIZE+12));

    if (mySocket.sockFDServer < 0)
    {
        perror("Error Creating Socket");
        return -1;
    }

    pthread_create(&mySocket.R, NULL, Recieve_Thread, NULL);
    pthread_create(&mySocket.S, NULL, Send_Thread, NULL);

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
    pthread_mutex_lock(&recieveBuffer);
    pthread_mutex_lock(&sendBuffer);
    mySocket.sockFDClient = accept(sockfd, addr, addrlen);
    mySocket.RshouldWait = 0;
    mySocket.SshouldWait = 0;
    pthread_mutex_unlock(&recieveBuffer);
    pthread_mutex_unlock(&sendBuffer);
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
    pthread_mutex_lock(&recieveBuffer);
    pthread_mutex_lock(&sendBuffer);
    mySocket.isServer = 0;
    mySocket.sockFDClient = mySocket.sockFDServer;
    mySocket.sockFDServer = 0;
    mySocket.RshouldWait = 0;
    mySocket.SshouldWait = 0;
    pthread_mutex_unlock(&recieveBuffer);
    pthread_mutex_unlock(&sendBuffer);

    if (connect(sockfd, addr, addrlen) < 0)
    {
        perror("Unable to Connect");
        return -1;
    }
    return 0;
}

int my_close(int fd)
{
    
    printf("Started Close Mechanism\n");
    pthread_mutex_lock(&recieveBuffer);
    pthread_mutex_lock(&sendBuffer);
    if (mySocket.isServer && mySocket.sockFDClient == fd)
    {
        printf("Sleep Started\n");
        mySocket.RshouldWait = 1;
        mySocket.SshouldWait = 1;
    }
    else if ((mySocket.isServer && mySocket.sockFDServer == fd) || (!mySocket.isServer && mySocket.sockFDClient == fd))
    {
        printf("Termination started\n");
        mySocket.RShouldTerminate = 1;
        mySocket.SShouldTerminate = 1;
    }
    else
    {
        perror("Socket Error");
        exit(1);
    }
    pthread_cond_signal(&recieveUpdated);
    pthread_cond_signal(&SendUpdated);
    pthread_mutex_unlock(&sendBuffer);
    pthread_mutex_unlock(&recieveBuffer);
    if ((mySocket.isServer && mySocket.sockFDServer == fd) || (!mySocket.isServer && mySocket.sockFDClient == fd))
    {
        pthread_join(mySocket.R, NULL);
        pthread_join(mySocket.S, NULL);
        for(int i=0;i<10;i++)
            free(mySocket.ReceiveMessage[i]);
        for(int i=0;i<10;i++)
            free(mySocket.SendMessage[i]);
        free(mySocket.ReceiveMessage);
        free(mySocket.SendMessage);
    }
    printf("Finished Closing\n");
    sleep(5);
    return close(fd);
}

ssize_t my_send(int sockfd, const void *buf, size_t len, int flags)
{
    char messageSize[50];
    sprintf(messageSize, "%ld", len);
    char newMessage [((int)len + 8 + (int)strlen(messageSize))];
    int i = 0;
    for (; i < (int)strlen(messageSize); i++)
    {
        newMessage[i] = messageSize[i];
    }
    for (; i < (int)strlen(messageSize) + 4; i++)
    {
        newMessage[i] = '#';
    }
    const char *message = buf;
    for (; i < (int)strlen(messageSize) + 4 + (int)len; i++)
    {
        newMessage[i] = message[i - 4 - strlen(messageSize)];
    }
    for (; i < (int)strlen(messageSize) + 4 + (int)len + 4; i++)
    {
        newMessage[i] = '#';
    }
    // printf("IN SEND -> MESSAGE IS ->\n");
    // for(int i = 0 ; i < get_size(newMessage); i++){
    //     printf("%c",newMessage[i]);
    // }
    // printf("\n");
    pthread_mutex_lock(&sendBuffer);
    while (mySocket.sendTableSize == MAX_TABLE_ENTRIES)
    {
        pthread_cond_wait(&SendUpdated, &sendBuffer);
    }
    // printf("hello");
    // mySocket.SendMessage[mySocket.sendTableSize] = (char *)realloc(mySocket.SendMessage[mySocket.sendTableSize], ((int)len + 8 + (int)strlen(messageSize))*sizeof(char));
    for (int i = 0; i < ((int)len + 8 + (int)strlen(messageSize))*sizeof(char); i++)
    {
        mySocket.SendMessage[mySocket.sendTableSize][i] = newMessage[i];
    }
    mySocket.sendTableSize++;
    // printf("IN SEND\n");
    // print_table(mySocket.SendMessage, mySocket.sendTableSize);
    pthread_cond_signal(&SendUpdated);
    pthread_mutex_unlock(&sendBuffer);
    return len;
}

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags)
{
    char size_char[50];
    pthread_mutex_lock(&recieveBuffer);
    while (mySocket.receiveTableSize < 1)
    {
        pthread_cond_wait(&recieveUpdated, &recieveBuffer);
    }
    char message[get_size(mySocket.ReceiveMessage[0])];
    for (int i = 0; i < get_size(mySocket.ReceiveMessage[0]); i++)
    {
        message[i] = mySocket.ReceiveMessage[0][i];
    }
    for (int i = 1; i < mySocket.receiveTableSize; i++)
    {
        mySocket.ReceiveMessage[i - 1] = (char *)realloc(mySocket.ReceiveMessage[i - 1], get_size(mySocket.ReceiveMessage[i]) * sizeof(char));
        for (int j = 0; j < get_size(mySocket.ReceiveMessage[i]); j++)
        {
            mySocket.ReceiveMessage[i - 1][j] = mySocket.ReceiveMessage[i][j];
        }
    }
    // free(mySocket.ReceiveMessage[mySocket.receiveTableSize - 1]);
    mySocket.receiveTableSize--;
    // printf("IN RECIEVE\n");
    // print_table(mySocket.ReceiveMessage, mySocket.receiveTableSize);
    pthread_cond_signal(&recieveUpdated);
    pthread_mutex_unlock(&recieveBuffer);
    char *buffer = buf;
    int i = 0;
    while (message[i] != '#')
    {
        size_char[i] = message[i];
        i++;
    }
    size_char[i] = '\0';
    int size = atoi(size_char);
    i = i + 4;
    int j = min(len + 4 + strlen(size_char), size + 4 + strlen(size_char));
    for (; i < j; i++)
    {
        buffer[i - 4 - strlen(size_char)] = message[i];
    }
    return min(len, size);
}

void *Recieve_Thread(void *params)
{
    char buffer[MAXSIZE];
    char *message;
    int isComplete = 0;
    int count = 0;
    int currSize = 0, pastSize = 0;
    int messageSize = MAXSIZE;
    struct pollfd poller;
    int poll_return = 0;
    while (1)
    {
        pthread_mutex_lock(&recieveBuffer);
        if (mySocket.RShouldTerminate == 1 && !poll_return)
        {
            pthread_mutex_unlock(&recieveBuffer);
            printf("Exiting From Recieve Thread\n");
            pthread_exit(0);
        }
        if (mySocket.RshouldWait == 1 && !poll_return)
        {
            // printf("HELLO");
            pthread_mutex_unlock(&recieveBuffer);
            continue;
        }
        pthread_mutex_unlock(&recieveBuffer);

        poller.fd = mySocket.sockFDClient;
        poller.events = POLLIN;

        poll_return = poll(&poller, 1, 3000);
        if (poll_return < 0)
        {
            perror("Unable to Poll...\n");
            pthread_exit(0);
        }
        if (poll_return == 0)
        {
            printf("POLLING\n");
            continue;
        }
        int n = recv(mySocket.sockFDClient, buffer, MAXSIZE, 0);
        if (n < 0)
        {
            // printf("Socket Error due to Socket %d returning %d", mySocket.sockFDClient, n);
            perror("Unable to Recieve");
            exit(1);
        }
        if (n == 0)
        {
            continue;
        }
        currSize += n;
        message = (char *)realloc(message, currSize);
        for (int i = pastSize; i < currSize; i++)
        {
            message[i] = buffer[i - pastSize];
        }
        if (currSize >= 8)
        {
            messageSize = get_size(message);
        }
        while (currSize >= messageSize)
        {
            int messageSize = get_size(message);
            char *temp = (char *)malloc(messageSize * sizeof(char));
            for (int i = 0; i < messageSize; i++)
            {
                temp[i] = message[i];
            }
            for (int i = messageSize; i < currSize; i++)
            {
                message[i - messageSize] = message[i];
            }
            message = (char *)realloc(message, currSize - messageSize);
            pthread_mutex_lock(&recieveBuffer);
            while (mySocket.receiveTableSize >= MAX_TABLE_ENTRIES)
            {
                pthread_cond_wait(&recieveUpdated, &recieveBuffer);
            }
            // mySocket.ReceiveMessage[mySocket.receiveTableSize] = (char *)realloc(mySocket.ReceiveMessage[mySocket.receiveTableSize], get_size(temp));
            for (int i = 0; i < get_size(temp); i++)
            {
                mySocket.ReceiveMessage[mySocket.receiveTableSize][i] = temp[i];
            }
            mySocket.receiveTableSize++;
            // printf("IN RECIEVE THREAD\n");
            // print_table(mySocket.ReceiveMessage, mySocket.receiveTableSize);
            currSize -= messageSize;
            pthread_cond_signal(&recieveUpdated);
            pthread_mutex_unlock(&recieveBuffer);
            if (currSize < 8)
            {
                messageSize = (int)1e5;
            }
        }
        pastSize = currSize;
    }
}
void *Send_Thread(void *params)
{
    while (1)
    {
        sleep(2);
        pthread_mutex_lock(&sendBuffer);
        if (mySocket.SShouldTerminate == 1)
        {
            pthread_mutex_unlock(&sendBuffer);
            printf("Exiting From Send Thread\n");
            pthread_exit(0);
        }
        if (mySocket.SshouldWait == 1)
        {
            pthread_mutex_unlock(&sendBuffer);
            continue;
        }
        while (mySocket.sendTableSize < 1 && mySocket.SShouldTerminate == 0 && mySocket.SshouldWait == 0)
        {
            pthread_cond_wait(&SendUpdated, &sendBuffer);
        }
        // printf("IN SEND THREAD\n");
        // print_table(mySocket.SendMessage, mySocket.sendTableSize);
        for (int i = 0; i < mySocket.sendTableSize; i++)
        {
            char *message = mySocket.SendMessage[i];
            int size = get_size(message);
            int curr = 0;
            char buffer[MAXSIZE];
            while (curr < size)
            {
                int i = 0;
                for (; i < 1000 && curr < size; i++)
                {
                    buffer[i] = message[curr++];
                }
                send(mySocket.sockFDClient, buffer, i, 0);
            }
            // free(mySocket.SendMessage[i]);
        }
        mySocket.sendTableSize = 0;
        pthread_cond_signal(&SendUpdated);
        pthread_mutex_unlock(&sendBuffer);
    }
}