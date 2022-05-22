/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Mem_Imp.hpp"

using namespace Ex4;

#define PORT "6666" // the port client will be connecting to 

#define SIZE 1024 // max number of bytes we can get at once 

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

///////////////////////////////////////////SENDING COMMAND///////////////////////////////////
void sendCommand(char * command, int sockfd){if (send(sockfd, command,strlen(command), 0)== -1) {perror("send");} }


void runCommand(char * buffer,char * token, int sockfd){
    char s[2]=" ";
    int numbytes;
    char buf[SIZE];
    token=strtok(NULL,s);
    //printf("TOKEN IS : %s\n",token);
    memset(buf,0,strlen(buf));
    if(!strcmp(buffer,"TOP")){
    //printf("entered TOP!!!\n");
    sendCommand("TOP",sockfd);
    if ((numbytes = recv(sockfd, buf, SIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("TOP: '%s'\n",buf);
    }
    else if(!strcmp(buffer,"POP")){
        sendCommand("POP",sockfd);
    
    }
    else if(!strcmp(buffer,"PUSH")){
        char string[SIZE];
        memset(string,0,strlen(string));
        while (token!=NULL){
            strcat(string,token); 
            if ((token=strtok(NULL,s))==NULL){break;} 
            strcat(string," ");
        }
        //Here send the string to the server.
        //printf("entered PUSH!!!\n");
        sendCommand("PUSH",sockfd);
        if ((numbytes = recv(sockfd, buf, SIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        if (!strcmp("GOT PUSH",buf)){
            //printf("Sending the string= %s \n",string);
            sendCommand(string,sockfd);
        }
    }
    else {
        printf("UNKNOWN COMMAND ENTERED!\n");
    }
}
/////////////////////////////////////////////CLIENT INPUT///////////////////////////////////
void communication(int sockfd){
    char buf[SIZE];
    int numbytes;
    char command[1024],buffer[SIZE];
    char *token,*string;
    const char s[2]=" ";
    memset(buf,0,strlen(buf));
    if ((numbytes = recv(sockfd, buf, SIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
    if (!strcmp("connected",buf))
    {
        while(1){
            printf("Enter STACK command: ");
            if (fgets(command,SIZE,stdin)==NULL){
                    printf("!Error in getting command!");
            }
            else{
                    int commandLen=strlen(command);
                    //string=(char *)calloc(commandLen,sizeof(char));
                    string=(char *)malloc(commandLen*sizeof(char));
                    memset(string,0,strlen(string));
                    memcpy(string,&command[0],commandLen);
                    string[commandLen-1]='\0';
                    if (!strlen(string))
                    continue;
                    memset(buffer,0,strlen(buffer));
                    token=strtok(string,s);
                    strcat(buffer,token);
                    runCommand(token,token,sockfd);
            }
        }
    }
    
}

//////////////////////////////////////MAIN/////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[SIZE];
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
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    communication(sockfd);
    return 0;
}*/

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

#define PORT "6666" 

#define MAXDATASIZE 100 

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool precmp (const char *pre, const char *str){return strncmp(pre, str, strlen(pre)) == 0;}

void *send_handler(void* sockfd) 
{
    size_t SIZE = 1024;
    char * input;
    int sock = *(int *)sockfd;
    std::cout << "enter a command(TOP, POP, PUSH): \n" << std::endl;
    while(true)
    {
        input = (char*)Ex4::Mem_Imp::calloc(SIZE, sizeof(char));
        getline(&input, &SIZE, stdin);
        if(precmp("PUSH",input)||precmp("POP",input)||precmp("TOP",input)){
            send(sock, input, SIZE, 0);
            std::cout << "enter a command(TOP, POP, PUSH): \n" << std::endl;
        }
        if (precmp("QUIT", input)){exit(0);}  
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

    freeaddrinfo(servinfo); // all done with this structure
    
    pthread_t client_thread;
    pthread_create(&client_thread, NULL, send_handler, &sockfd);
    
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
            printf("client: received %s\n",output);   
        }
    } 
    
    close(sockfd);

    return 0;
}  