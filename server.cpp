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
#include "Stack.hpp"
#include <fcntl.h>
#include <sys/mman.h>

#define PORT "6666" 
#define BACKLOG 10 
#define SIZE 1024
int fd;
struct my_stack
{
    char data[SIZE];
    int index;
};

my_stack * share_stack; 
struct flock lock;

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
    char command[1024];
    int *sock = (int*) tempSock;
    int new_sock = *sock;
    if (send(*sock, "CONNECTED", 10, 0) == -1)
    {
        perror("send"); 
    }      
    while (true)
    {
        memset(command,0,strlen(command));
        if ((recv(*sock, command, SIZE, 0)) !=-1) 
        {
             if (precmp("QUIT",command))
            { 
                printf("DEBUG: CLOSE CONNECTION WITH CLIENT \n");
                break;
            }
            lock.l_type = F_WRLCK;   
            fcntl(fd, F_SETLKW, &lock);
            if(precmp("POP",command))
            {
                char * poped = share_stack[share_stack->index - 1].data;
                if (precmp("NULL",poped))
                {
                    strcpy(share_stack[share_stack->index+1].data, "NULL");
                    if (send(new_sock,"DEBUG:STACK IS EMPTY",21, 0)== -1)
                    {
                        perror("send");
                    }
                    std::cout << "DEBUG:STACK IS EMPTY" << std::endl;
                    lock.l_type = F_UNLCK; //unlocking the lock in case of underflow
                    fcntl (fd, F_SETLKW, &lock);
                    continue;
                }
                strcpy(share_stack[share_stack->index--].data, "");
                if (send(new_sock,"POP COMPLETED",14, 0)== -1){
                      perror("send"); 
                }
                std::cout << "OUTPUT:" << "POP COMPLETED" << std::endl;
            }
            else if(precmp("PUSH",command))
            {
                memcpy(command, command + 4, 1020);
                strcpy(share_stack[share_stack->index++].data, command);
                if (send(new_sock,"PUSH COMPLETED",15, 0)== -1){
                        perror("send"); 
                    }
                std::cout << "OUTPUT:" << "POP COMPLETED" << std::endl;
             }
            else if(precmp("TOP",command))
            {
                char * toped = share_stack[share_stack->index - 1].data;

                if (precmp("NULL",toped))
                {
                    std::cout << "DEBUG:STACK IS EMPTY"<< std::endl;
                    if (send(new_sock, "DEBUG:STACK IS EMPTY\n",22,0) == -1)
                    {
                        perror("send");
                    }
                    lock.l_type = F_UNLCK; //unlocking the lock in case of underflow
                    fcntl (fd, F_SETLKW, &lock);
                    continue;
                }
                else
                {
                    if (send(new_sock, toped,strlen(toped),0) == -1)
                    {
                        perror("send");
                    }
                }

            }
            lock.l_type = F_UNLCK; 
            fcntl (fd, F_SETLKW, &lock);
    
        }
    }
    close(new_sock);
    close(fd);
    return NULL;
}


//the code was wrriten with the help of the internet

int main(void)
{
    fd = open("shared_file.txt",O_WRONLY|O_CREAT);
    if (fd == -1)
    {
        perror("DEBUG: FILE CREATION FAILED");
    }
    memset(&lock, 0, sizeof(lock)); //initializing the lock
    share_stack = (my_stack *)mmap(NULL, 10240, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    if (share_stack == MAP_FAILED)
    {
        printf("DEBUG: MAPPING STACK FAILED\n");
        return 1;
    }
    strcpy(share_stack[share_stack->index++].data, "NULL");
    int sock, new_sock;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    struct sigaction sa;
    int connect=1;
    int rv;
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
    int j=0;
    while(1) {
        j++;  
        sin_size = sizeof their_addr;
        new_sock = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
        if (new_sock == -1) {
            perror("accept");
            continue;
        }
        std::cout << "CLIENT CONNECTED TO THE SERVER\n" << std::endl;
        if (!fork()) {
            close(sock);
            send_handler(&new_sock);
            close(new_sock);
            exit(0);
        }
        close(new_sock);
    }
    
    return 0;
}
