#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
//#include "Mem_Imp.hpp"
//using namespace Ex5;

#define PORT "6666" 


void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool precmp (const char *pre, const char *str){return strncmp(pre, str, strlen(pre)) == 0;}

// void *send_handler(void* socket) 
// {
//     size_t SIZE = 1024;
//     char * input;
//     int sock = *(int *)socket;
//     std::cout << "ENTER A COMMAND (TOP, POP, PUSH, QUIT): \n" << std::endl;
//     while(true)
//     {
//         input = (char*)Ex5::Mem_Imp::calloc(SIZE, sizeof(char));
//         getline(&input, &SIZE, stdin);
//         if(precmp("PUSH",input)||precmp("POP",input)||precmp("TOP",input)){
//             send(sock, input, SIZE, 0);
//             sleep(1);
//             std::cout << "ENTER A COMMAND (TOP, POP, PUSH,QUIT): \n" << std::endl;
//         }
//         if (precmp("QUIT", input)){
//             send(sock, "QUIT", SIZE, 0);
//             exit(0);}  
//     }
//  }

//the code was wrriten with the help of the internet
int main(int argc, char *argv[])
{
    int sock;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("CLIENT CONNECTED TO %s\n", s);

    freeaddrinfo(servinfo); 
    
    // pthread_t client_thread;
    // pthread_create(&client_thread, NULL, send_handler, &sock);
    
    char * output;
    output = (char*)calloc(1024, sizeof(char));

    while (true)
    {
        memset(output,0,strlen(output));
        printf("Enter Messege: ");
        fgets(output, 1024, stdin);
        send(sock, output, 1024, 0);
        memset(output,0,strlen(output));
        if ((recv(sock, output, 1024, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        else 
        {
            printf("OUTPUT: %s\n", output);
        }
        

    } 
    
    close(sock);

    return 0;
}  

