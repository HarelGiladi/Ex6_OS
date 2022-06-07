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
#include <fcntl.h>
#include <sys/mman.h>

#include "AO.hpp"

#define PORT "6666" 
#define BACKLOG 10 
#define SIZE 1024


Queue *q_1 = Queue::createQ();
Queue *q_2 = Queue::createQ();
Queue *q_3 = Queue::createQ();


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

bool isValid(char* text) {
    for (size_t i = 0; i < strlen(text); i++)
    {
        if (!isprint(text[i])) {
            return false;
        }
    }
    return true;    
}

void* enQ_to_2(void* x) {
    q_2->enQ(x);
    return x;
}

void* enQ_to_3(void* x) {
    q_3->enQ(x);
    return x;
}

//https://www.tutorialspoint.com/cplusplus-program-to-implement-caesar-cypher
void* caesarCypher (void* data) {
    std::cout<<"cypher"<< (char*)data<<std::endl;
    char ch;
    int key = 1;
    char* msg ;
    msg = (char *) data;

    for(int i = 0; msg[i] != '\0'; ++i) {
         ch = msg[i];

         //encrypt for lowercase letter
         if (ch >= 'a' && ch <= 'z'){
            ch = ch + key;
            if (ch > 'z') {
               ch = ch - 'z' + 'a' - 1;
            }  
            msg[i] = ch;
         }
         //encrypt for uppercase letter
         else if (ch >= 'A' && ch <= 'Z'){
            ch = ch + key;
            if (ch > 'Z'){
               ch = ch - 'Z' + 'A' - 1;
            }
            msg[i] = ch;
         }
      }
    std::cout<<"CC"<<msg<<std::endl;
    return data;
}

//https://www.geeksforgeeks.org/convert-alternate-characters-string-upper-case/
void* convert (void* data) {
    std::cout<<"convert"<<std::endl;
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
    std::cout<<"FF"<<msg<<std::endl;
    return data;
}



void* send_to(void* x) {
    cout<<"send"<<endl;
    char* data = (char*)x;
    cout<<data<<endl;
    if (send(q_3->get_fd(), x, strlen(data), 0) < 0) {
        cout<<"error"<<endl;
        perror("error");
        exit(1);
    }
    cout<<"done"<<endl;
    return x;
}

static void * handle(void *tempSock) {
    
    int* sock = (int*) tempSock;
    int new_sock = *sock;
    char str[SIZE];
    active_object* ao_1 , *ao_2, *ao_3; 
   
    while (1) {
        memset(str, 0,SIZE);
        recv(new_sock,str,SIZE,0);
        cout << "im here 1\n";fflush(stdout);
        cout << str;fflush(stdout);
        q_1->enQ(str);
        cout << (char*)q_1->front->data;
        q_3->enQ(str);
        cout << (char*)q_3->front->data;
        q_3->set_fd(new_sock);
        if (!(strcmp(str, "QUIT\n"))) {
            break;
        }        
        cout << "im here 2\n";fflush(stdout);
        ao_1 = newAO(q_1, caesarCypher, enQ_to_2);
        cout << "im here 3\n";fflush(stdout);
        ao_2 = newAO(q_2, convert, enQ_to_3);
        cout << "im here 4\n";fflush(stdout);
        ao_3 = newAO(q_3, send_to, NULL);
        cout << "im here 5\n";fflush(stdout);
        
    }
    destroyAO(ao_1);
    destroyAO(ao_2);
    destroyAO(ao_3);
    
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
       
        pthread_t thread;
        int * ptr = (int*) malloc (sizeof(int));
        *ptr = new_sock;
        if (pthread_create(&thread, NULL, handle, ptr) != 0) { 
            perror("start thread");
            close(new_sock);
        }
    }
    
    return 0;
}
