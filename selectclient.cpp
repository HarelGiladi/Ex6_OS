#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>



int connected = 0;
int sock = -1;

void *recv(void *arg)
{
    char buff[1024] = {0};
    connected = 1;
    int bytes = 0;
    while ((bytes = recv(sock, buff, 1024, 0)) != -1)
    {
        if (!bytes)
        {
            connected = 0;
            break;
        }
        printf("RECV: %s\n",buff);
        if (!strcmp(buff, "QUIT"))
        {
            connected = 0;
            break;
        }
        bzero(buff, 1024);
    }
    return NULL;
}

void *send(void *arg)
{
    char input[1024] = {0};
    while (connected != 0)
    {
        scanf("%s", input);
        printf("SEND: %s\n",input);
        if (strncmp(input,"QUIT",4) == 0)
        {
            send(sock,"exit",4,0);
            connected = 0;
            break;
        }
        if (send(sock, input, strlen(input) + 1, 0) == -1)
        {
            perror("send");
        }
        bzero(input, 1024);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    printf("WELCOME\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return -1;
    }
    signal(SIGPIPE, SIG_IGN); 

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(6666); 

    int clientSocket = connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (clientSocket == -1)
    {
        printf("listen failed");
        close(sock);
        return -1;
    }

    pthread_t container[2];
    connected = 1;
    pthread_create(&container[0], NULL, recv, NULL);
    pthread_create(&container[1], NULL, send, NULL);
    pthread_join(container[1], NULL);
    pthread_kill(container[1], 0);

    close(sock);
    return 0;
}