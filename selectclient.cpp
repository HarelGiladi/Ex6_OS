#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#define PORT 6667
int sock;

void* receive_handler(void*){
    char buf[4096];
    while(1){
        if (recv(sock, buf, 4096, 0) == -1) {
            printf("receiving from server failed...\n");
            exit(1);
        }
        printf("%s", buf);
        fflush(stdout);
    }
}

void* send_handler(void*){
    char buf[4096];
    unsigned int len = 4096;
    while(1){
        fgets(buf, 4096, stdin);
        size_t ln = strlen(buf) - 1;
        if (buf[ln] == '\n') {
            buf[ln] = '\0';
        }
        if (send(sock, buf, 4096, 0) == -1) {
            printf("sending to the server failed...\n");
            exit(1);
        }
    }
}

int main(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(1);
    }
    printf("CONNECTED TO THE SERVER..\n");

    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, send_handler, NULL);
    pthread_create(&t2, NULL, receive_handler, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(sock);

    return 0;
}