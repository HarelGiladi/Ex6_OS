#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "Stack.hpp"

#define PORT "6666" 
#define BACKLOG 10 
#define SIZE 1024

pthread_mutex_t mutex;


Ex4::Stack Stack;

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

bool precmp (const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void * sendMessage(void * tempSock)
{
    char* command;
    int sock = *(int*) tempSock;
    if (send(sock, "connected", strlen("connected"), 0) == -1){
        perror("send"); 
    }
    command = (char *)Ex4::Mem_Imp::malloc(SIZE);
    
    
    while (true)
    {
        memset(command,0,strlen(command));
        if ((recv(sock, command, SIZE, 0)) != 0) 
        {
          
            pthread_mutex_lock(&mutex);
        
            if(precmp("POP",command))
            {
                Stack.POP();
                 if (send(sock,"POP COMPLETED",15, 0)== -1){
                    perror("send"); 
                }
                std::cout << "POP COMPLETED\n" << std::endl;
                //char* out = Stack.POP();
                // if (send(sock,Stack.POP(),strlen(Stack.POP()), 0)== -1){
                //     perror("send"); 
                // }
            }

            else if(precmp("TOP",command))
            {
                char* ans = (char *)Ex4::Mem_Imp::calloc(SIZE, sizeof(char));
                ans = Stack.TOP();
                if (send(sock,ans,strlen(ans), 0)== -1){
                    perror("send"); 
                }
                std::cout << ans << std::endl;

                
            }

            else if(precmp("PUSH",command))
            {
                char* substr = (char*)Ex4::Mem_Imp::calloc(strlen(command),sizeof(char));
                strncpy(substr, command+4, strlen(command)-4);
                Stack.PUSH(substr);
                 if (send(sock,"PUSH COMPLETED",16, 0)== -1){
                    perror("send"); 
                }
                std::cout << "PUSH COMPLETED\n" << std::endl;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}


int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int connect=1;
    int rv;
    
    pthread_mutex_init(&mutex,NULL);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &connect,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    std::cout << "waiting for connections...\n" << std::endl;
    pthread_t threads[50];
    int j=0;
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        std::cout << "client connected to the server\n" << std::endl;
        if(pthread_create(&threads[j++],NULL,sendMessage,&new_fd)!=0){
            printf("Thread creation error!\n");
        }
        if(j>=50){
            j=0;
            while (j<50)
            {
                pthread_join(threads[j++],NULL);
            }
            j=0;
        } 
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
