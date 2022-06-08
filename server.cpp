#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/signal.h>
#include <features.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stddef.h>
#include "AO.hpp"
// #include "Queue.hpp"

#define PORT "6666"
#define BACKLOG 10

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

Queue *q_1 = new Queue();
Queue *q_2 = new Queue();
Queue *q_3 = new Queue();
active_object* ao_1 = nullptr, *ao_2, *ao_3;    



void enQ_to_2(void* x) {
    q_2->enQ(x);
}

void enQ_to_3(void* x) {
    q_3->enQ(x);
}


//https://www.tutorialspoint.com/cplusplus-program-to-implement-caesar-cypher

void caesarCypher (void* data) {

    char ch;
    int key = 1;
    char* msg = (char*) data;

    for(int i = 0; msg[i] != '\0'; ++i) {
         ch = msg[i];

         if (ch >= 'a' && ch <= 'z'){
            ch = ch + key;
            if (ch > 'z') {
               ch = ch - 'z' + 'a' - 1;
            }  
            msg[i] = ch;
         }
         else if (ch >= 'A' && ch <= 'Z'){
            ch = ch + key;
            if (ch > 'Z'){
               ch = ch - 'Z' + 'A' - 1;
            }
            msg[i] = ch;
         }
      }
}
//https://www.geeksforgeeks.org/convert-alternate-characters-string-upper-case/

void convert (void* data) {
    char ch;
    char* msg = (char*) data;
    for (int i = 0; msg[i] != '\0'; ++i) {
        ch = msg[i];  
        //Convert lowercase to uppercase
        if (ch >= 'a' && ch <= 'z'){
            ch = (ch - 'a') + 'A';
            msg[i] = ch;
        }
        // Convert uppercase to lowercase
        else if (ch >= 'A' && ch <= 'Z') {
                ch = (ch - 'A') + 'a';
                msg[i] = ch;
        }
    }
    

}



void send_to(void* x) {
    char* data = (char*)x;
    if (send(*q_3->get_fd(), data, strlen(data), 0) < 0) {
        perror("sending");
        exit(1);
    }
}

static void * handler(void *tempSock) {
    
    int* sock = (int*) tempSock;
    int new_sock = *sock;
    char input[1024];
    while (1) {
        recv(new_sock,input,1024,0);
        q_1->enQ(input);
        q_3->set_fd(new_sock);
        if (!(strcmp(input, "QUIT\n"))) {
            break;
        }
        // if (!ao_1) {
        //     ao_1 = newAO(q_1, caesarCypher, enQ_to_2);
        //     ao_2 = newAO(q_2, convert, enQ_to_3);
        //     ao_3 = newAO(q_3, send_to, NULL);
        // }
    }
    destroyAO(ao_1);
    destroyAO(ao_2);
    destroyAO(ao_3);
    
    return NULL;
}


void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

//the code was wrriten with the help of the internet
//and the class material
int main(void) {
    ao_1 = newAO(q_1, caesarCypher, enQ_to_2);
    ao_2 = newAO(q_2, convert, enQ_to_3);
    ao_3 = newAO(q_3, send_to, NULL);
    int sock, new_sock;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    struct sigaction sa;
    int connect = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
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

    if (p == NULL) {
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

    while(1) {  
        sin_size = sizeof their_addr;
        new_sock = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
        if (new_sock == -1) {
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);

        std::cout << "CLIENT CONNECTED TO THE SERVER\n" << std::endl;
        
        pthread_t t;
        if (pthread_create(&t, NULL, handler, &new_sock) != 0) { 
            perror("start thread");
            close(new_sock);
        }
        
    }

    return 0;
}