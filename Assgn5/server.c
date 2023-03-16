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

void test_count(int ClientSock){
    char message[10000];
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    char input1[] = "Message Number: 1";
    my_send(ClientSock, input1, strlen(input1)+1, 0);
    printf("Message Sent\n");
    char input2[] = "Message Number: 2";
    my_send(ClientSock, input2, strlen(input2)+1, 0);
    printf("Message Sent\n");
    char input3[] = "Message Number: 3";
    my_send(ClientSock, input3, strlen(input3)+1, 0);
    printf("Message Sent\n");
    char input4[] = "Message Number: 4";
    my_send(ClientSock, input4, strlen(input4)+1, 0);
    printf("Message Sent\n");
    char input5[] = "Message Number: 5";
    my_send(ClientSock, input5, strlen(input5)+1, 0);
    printf("Message Sent\n");
    char input6[] = "Message Number: 6";
    my_send(ClientSock, input6, strlen(input6)+1, 0);
    printf("Message Sent\n");
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    char input7[] = "Message Number: 7";
    my_send(ClientSock, input7, strlen(input7)+1, 0);
    printf("Message Sent\n");
    char input8[] = "Message Number: 8";
    my_send(ClientSock, input8, strlen(input8)+1, 0);
    printf("Message Sent\n");
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    char input9[] = "Message Number: 9";
    my_send(ClientSock, input9, strlen(input9)+1, 0);
    printf("Message Sent\n");
    char input10[] = "Message Number: 10";
    my_send(ClientSock, input10, strlen(input10)+1, 0);
    printf("Message Sent\n");
    char input11[] = "Message Number: 11";
    my_send(ClientSock, input11, strlen(input11)+1, 0);
    printf("Message Sent\n");
    char input12[] = "Message Number: 12";
    my_send(ClientSock, input12, strlen(input12)+1, 0);
    printf("Message Sent\n");
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    char input13[] = "Message Number: 13";
    my_send(ClientSock, input13, strlen(input13)+1, 0);
    printf("Message Sent\n");
    char input14[] = "Message Number: 14";
    my_send(ClientSock, input14, strlen(input14)+1, 0);
    printf("Message Sent\n");
    char input15[] = "Message Number: 15";
    my_send(ClientSock, input15, strlen(input15)+1, 0);
    printf("Message Sent\n");
    char input16[] = "Message Number: _6";
    my_send(ClientSock, input16, strlen(input16)+1, 0);
    printf("Message Sent\n");
    char input17[] = "Message Number: 17";
    my_send(ClientSock, input17, strlen(input17)+1, 0);
    printf("Message Sent\n");
    char input18[] = "Message Number: 18";
    my_send(ClientSock, input18, strlen(input18)+1, 0);
    printf("Message Sent\n");
    char input19[] = "Message Number: 19";
    my_send(ClientSock, input19, strlen(input19)+1, 0);
    printf("Message Sent\n");
    my_close(ClientSock);
}


void test_length(int ClientSock){
    char message[10000];
    my_recv(ClientSock, message, 10000, 0);
    printf("Message Recieved_==>  %s\n",message);
    
    FILE* ptr;
	char ch = 'a';
	ptr = fopen("large_input.txt", "r");
	if (NULL == ptr) {
		printf("file can't be opened \n");
	}
    char input[] = "hello ";
	while(ch != EOF){
        strncat(input, &ch, 1);
		ch = fgetc(ptr);
    }
	fclose(ptr);
    
    // input is of length 8002
    my_send(ClientSock, input, strlen(input)+1, 0);
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
    // ClientSock = my_accept(ServSock, (struct sockaddr *)& client_addr, &client_len);
    // test(ClientSock);
    ClientSock = my_accept(ServSock, (struct sockaddr *)& client_addr, &client_len);
    test_count(ClientSock);
    // ClientSock = my_accept(ServSock, (struct sockaddr *)& client_addr, &client_len);
    // test_length(ClientSock);
    my_close(ServSock);
    return 0;
}

