
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <iostream>

#include "Mem_Imp.hpp"
using namespace Ex4;
using namespace std;


#define PORT 6666

bool precmp (const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0; 
}

int main()
{
    size_t SIZE  = 1024;
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket creation failed");
        exit(1);
    }
    cout << "START" << endl;   

    size_t serv_size = sizeof(server_address);
    
    if (connect(sock,(struct sockaddr*)&server_address, serv_size)!= 0)
    {
        perror("socket connection failed");
        exit(1);
    }
    cout << "connected..." << endl;
    
    char * input;
    input = (char*)Ex4::Mem_Imp::calloc(SIZE, sizeof(char));
    recv(sock, input, 1024 ,0);

    send(sock, "PUSH HI" , 8, 0);
    recv(sock, input, 1024 ,0);
    
    send(sock, "PUSH this is a test", 20, 0);
    recv(sock, input, 1024 ,0);

    send(sock, "TOP", 4, 0 );
    recv(sock, input, 16 ,0);
    assert(precmp(" this is a test", input));
    
    send(sock, "POP" , 1024, 0);
    recv(sock, input, 1024, 0);
    assert(precmp(input, "POP COMPLETED"));

    send(sock, "TOP", 1024, 0);
    recv(sock, input, 1024, 0);
    assert(precmp(" HI", input));
    
    send(sock, "PUSH end", 1024, 0);
    recv(sock, input, 1024 ,0);
    
    send(sock, "TOP", 1024, 0);
    recv(sock, input, 1024, 0);
    if (precmp(" end",input)){cout << "TEST COMPLETED SUCCESSFULLY" << endl;}
}