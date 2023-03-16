#include "mysocket.h"
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = my_socket(AF_INET, SOCK_MyTCP, 0);
    printf("Socket Created\n");

    struct sockaddr_in Server_Addr;
    Server_Addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &Server_Addr.sin_addr);
    Server_Addr.sin_port = htons(20000);

    my_connect(sockfd, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));
    char *message = "Hello World";
    my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent\n");
    message = "Hello World_1";
    my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent\n");
    message = "Hello World_2";
    my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent\n");
    message = "Hello World_3";
    my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent\n");
    message = "Hello World_4";
    my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent\n");
    message = "Hello World_5";
    my_send(sockfd, message, strlen(message)+1, 0); 
    printf("Message Sent\n");
    char input[100];
    my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s\n",input);
    my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s\n",input);
    my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s\n",input);
    my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s\n",input);
    my_close(sockfd);

}