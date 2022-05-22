#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "Mem_Imp.hpp"
using namespace Ex4;

#define PORT "6666" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool precmp (const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void *T_FUNCTION(void* sockfd) 
{
    size_t SIZE = 1024;
    char * input;
    int sock = *(int *)sockfd;
    while(true)
    {

        input = (char*)Ex4::Mem_Imp::calloc(SIZE, sizeof(char));

        getline(&input, &SIZE, stdin);
        
        send(sock, input, SIZE, 0);

        if (precmp("QUIT", input))
        {
            exit(0);
        }
       
    }
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
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
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
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
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); 
    
    pthread_t client_thread;
    pthread_create(&client_thread, NULL, T_FUNCTION, &sockfd);
    
    char * output;
    output = (char*)Ex4::Mem_Imp::calloc(1024, sizeof(char));

    while (true)
    {
        memset(output,0,strlen(output));

        if ((numbytes = recv(sockfd, output, 1024, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        else 
        {
            printf("OUTPUT: %s\n",output);   
        }
    } 
    
    close(sockfd);

    return 0;
}