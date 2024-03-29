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
    char input[100];
    int n;
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);
    char test_size[5];
    n = my_recv(sockfd, test_size, 5, 0);
    printf("Message Recieved_ ==> ");
    for(int i = 0 ; i < 5; i++){
        printf("%c",test_size[i]);
    }
    printf(" of %d bytes", n);
    printf("\n");
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    message = "Hello World_1";
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);
    message = "Hello World_2";
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);

    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);

    message = "Hello World_3";
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);

    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);



    message = "Hello World_4";
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);
    message = "Hello World_5";
    n = my_send(sockfd, message, strlen(message)+1, 0);
    printf("Message Sent of %d bytes\n",n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    n = my_recv(sockfd, input, 100, 0);
    printf("Message Recieved_==>  %s of %d bytes\n",input,n);
    my_close(sockfd);

}