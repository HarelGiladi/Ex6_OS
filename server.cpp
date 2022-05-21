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

#define PORT "6666"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold
#define SIZE 1024

/*
*****************************************

Stack implementation

*****************************************
*/

pthread_mutex_t mutex;

// typedef struct Node
// {
//     char data[1024];
//     struct Node* next;
// }Stack;
// Stack *head=NULL;

// ////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

// char * TOP() 
// {
//     if (head == NULL) {
//         char * output = "STACK IS EMPTY";
//         return "STACK IS EMPTY";
//     } else {
//         return head->data;
//     }
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////////PUSH COMMAND///////////////////////////////////

// void PUSH(char * value) 
// {
//     Stack *newNode;
//     //malloc implemented by us
//     newNode = (struct Node*)malloc(sizeof(struct Node));
//     memset(newNode,0,sizeof(struct Node));
//     strcpy(newNode->data,value);
//     if (head == NULL) {
//         newNode->next = NULL;
//     } else {
//         newNode->next = head;
//     }
//     head = newNode;
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

// void POP() 
// {
//     if (head == NULL) {
//         //do nothing
//     } else {
//         Stack *temp = head;
//         head = head->next;
//         free(temp);
//     }
// }

/////////////////////////////////////////////////////////////////////////////////////////////SERVER///////////////////////////////////////////////////////////////////////////////

Ex4::Stack Stack;

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void * sendMessage(void * tempSock)
{
    char command[SIZE];
    int numbytes;
    char buf2[SIZE];
    int numbytes2;
    int sock = *(int*) tempSock;
    if (send(sock, "connected", strlen("connected"), 0)== -1)
        perror("send"); 
    int live=1;
    while (live)
    {
     memset(command,0,strlen(command));
     if ((numbytes = recv(sock, command, SIZE-1, 0)) == -1) {
        perror("recv");
        //exit(1);
     }
    command[numbytes] = '\0';
    memset(buf2,0,strlen(buf2));

    ////////////////////////////////////////////////////////////////////////////////////////////////////////POP COMMAND///////////////////////////////////

    pthread_mutex_lock(&mutex);
    if (strlen(command)==0)
    {
        live=0;
        close(sock);
     }
    else if(!strcmp("POP",command)){
    Stack.POP();
    if (send(sock,"POPPED",strlen("POPPED"), 0)== -1)
        perror("send"); 
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

    else if(!strcmp("TOP",command)){
    //send the string at the top.
    if (send(sock,Stack.TOP(),strlen(Stack.TOP()), 0)== -1)
        perror("send"); 
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////PUSH COMMAND///////////////////////////////////

    else if(!strcmp("PUSH",command)){
     if (send(sock, "GOT PUSH",strlen("GOT PUSH"), 0)== -1)
        perror("send"); 
     if ((numbytes2 = recv(sock, buf2, SIZE-1, 0)) == -1) {
        perror("recv");
        close(sock);
        live=0;
     }
    buf2[numbytes2] = '\0';
    Stack.PUSH(buf2);
    }
    pthread_mutex_unlock(&mutex);
    }
    while(1){sleep(1000000000);}
}

/*
*****************************************

malloc and free implementation

from - https://stackoverflow.com/questions/5422061/malloc-implementation

*****************************************
*/

// typedef struct free_block {
//     size_t size;
//     struct free_block* next;
// } free_block;

// static free_block free_block_list_head = { 0, 0 };
// //static const size_t overhead = sizeof(size_t);
// static const size_t align_to = 16;


// void* malloc(size_t size) {
//     size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
//     free_block* block = free_block_list_head.next;
//     free_block** head = &(free_block_list_head.next);
//     while (block != 0) {
//         if (block->size >= size) {
//             *head = block->next;
//             return ((char*)block) + sizeof(size_t);
//         }
//         head = &(block->next);
//         block = block->next;
//     }

//     block = (free_block*)sbrk(size);
//     block->size = size;

//     return ((char*)block) + sizeof(size_t);
// }

// void free(void* ptr) {
//     free_block* block = (free_block*)(((char*)ptr) - sizeof(size_t));
//     block->next = free_block_list_head.next;
//     free_block_list_head.next = block;
// }

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
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

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
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

    printf("server: waiting for connections...\n");
    pthread_t threads[50];
    int j=0;
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        //printf("thread value : %p\n",&threads[j]);
        //printf("SOCK FD VALUE = %p\n",&new_fd);
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
        // pthread_exit(NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
