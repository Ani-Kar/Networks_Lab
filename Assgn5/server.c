#include "mysocket.h"
#include <stdio.h>
#include <netinet/in.h>

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

    char message[100];
    my_recv(ClientSock, message, 100, 0);
    printf("Message Recieved_==>  %s\n",message);
    char *message_ = "Hello World";
    my_send(ClientSock, message_, strlen(message_)+1, 0);
    printf("Message Sent\n");
    my_close(ServSock);
    return 0;
}

