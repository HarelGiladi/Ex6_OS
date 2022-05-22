#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <pthread.h>
#include "Stack.hpp"

#define PORT "6666" 
#define BACKLOG 10 
#define SIZE 1024

pthread_mutex_t mutex;


Ex4::Stack Stack;

void sigchld_handler(int s)
{
    
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

bool precmp (const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void * send_handler(void * tempSock)
{
    char* command;
    int sock = *(int*) tempSock;
    if (send(sock, "CONNECTED", 10, 0) == -1){
        perror("send"); 
    }
    command = (char *)Ex4::Mem_Imp::malloc(SIZE);
    
    
    while (true)
    {
        memset(command,0,strlen(command));
        if ((recv(sock, command, SIZE, 0)) !=-1) 
        {
          
            pthread_mutex_lock(&mutex);
        
            if(precmp("POP",command))
            {
                if(Stack.IsEmpty())
                {
                   if (send(sock,"DEBUG:STACK IS EMPTY",21, 0)== -1)
                   {
                    perror("send");
                   }
                   std::cout << "DEBUG:STACK IS EMPTY" << std::endl;

                }
                else
                {
                    Stack.POP();
                    if (send(sock,"POP COMPLETED",15, 0)== -1){
                        perror("send"); 
                    }
                    std::cout << "OUTPUT:" << "POP COMPLETED" << std::endl;
                }
            }

            else if(precmp("TOP",command))
            {
                char* ans = (char *)Ex4::Mem_Imp::calloc(SIZE, sizeof(char));
                ans = Stack.TOP();
                if (send(sock,ans,strlen(ans), 0)== -1){
                    perror("send"); 
                }
                std::cout << "OUTPUT:" << ans << std::endl;

                
            }

            else if(precmp("PUSH",command))
            {
                char* substr = (char*)Ex4::Mem_Imp::calloc(strlen(command),sizeof(char));
                strncpy(substr, command+4, strlen(command)-4);
                Stack.PUSH(substr);
                 if (send(sock,"PUSH COMPLETED",16, 0)== -1){
                    perror("send"); 
                }
                std::cout << "OUTPUT:" << "PUSH COMPLETED" << std::endl;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}


//the code was wrriten with the help of the internet
int main(void)
{
    int sock, new_sock;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    struct sigaction sa;
    int connect=1;
    int rv;
    
    pthread_mutex_init(&mutex,NULL);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &connect,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); 

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; 
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    std::cout << "WAITING FOR CONNECTIONS....\n" << std::endl;
    pthread_t threads[20];
    int j=0;
    while(1) {  
        sin_size = sizeof their_addr;
        new_sock = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
        if (new_sock == -1) {
            perror("accept");
            continue;
        }

        std::cout << "CLIENT CONNECTED TO THE SERVER\n" << std::endl;
        if(pthread_create(&threads[j++],NULL,send_handler,&new_sock)!=0){
            printf("Thread creation error!\n");
        }
        if(j>=20){
            j=0;
            while (j<20){pthread_join(threads[j++],NULL);}
            j=0;
        } 
    }
    pthread_mutex_destroy(&mutex);
    for (size_t i = 0; i < 20; i++)
    {
        pthread_cancel(threads[i]);
    }
    
    return 0;
}
