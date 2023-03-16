#include "mysocket.h"
#include <stdio.h>
#include <netinet/in.h>
void test(int ClientSock){
    char message[10000];
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    char input[] = "Hello World";
    my_send(ClientSock, input, strlen(input)+1, 0);
    printf("Message Sent\n");
    char input1[] = "Hello World_1";
    my_send(ClientSock, input1, strlen(input1)+1, 0);
    printf("Message Sent\n");
    char input2[] = "Hello World_2";
    my_send(ClientSock, input2, strlen(input2)+1, 0);
    printf("Message Sent\n");
    char input3 [] = "Hello World_3";
    my_send(ClientSock, input3, strlen(input3)+1, 0);
    printf("Message Sent\n");
    my_close(ClientSock);
}

int main()
{
    int ServSock,ClientSock;
    struct sockaddr_in client_addr, server_addr;

    ServSock = my_socket(AF_INET, SOCK_MyTCP, 0);
    printf("Socket Created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(20000);

    my_bind(ServSock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    printf("Socket Binded\n");

    my_listen(ServSock, 10);
    printf("Socket Listening\n");

    int client_len = sizeof(client_addr);
    ClientSock = my_accept(ServSock, (struct sockaddr *)& client_addr, &client_len);
    test(ClientSock);
    ClientSock = my_accept(ServSock, (struct sockaddr *)& client_addr, &client_len);
    test(ClientSock);
    my_close(ServSock);
    return 0;
}

