#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include "reactor.hpp"
#include <pthread.h>

#define PORT "6666" 
int fd_count;
struct pollfd *pfd;
int listener;
char buf[1024];

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int get_listener_socket(void)
{
    int listener; 
    int yes = 1; 
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai);

    if (p == NULL)
    {
        return -1;
    }

    if (listen(listener, 10) == -1)
    {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2; // Double it

        *pfds = (pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count - 1];

    (*fd_count)--;
}

void *handler(void *arg)
{
    reactor* r = (reactor*)arg;
    int new_fd = r->fd;
    while (1)
    {
        int bytes = recv(new_fd, buf, sizeof(buf), 0);
        if (bytes <= 0)
        {
            printf("CLIENT %d LEFT\n", new_fd);
            close(new_fd);
            return NULL;
        }
        else
        {
            for (int i = 0; i < fd_count + 1; i++)
            {
                int client_fd = pfd[i].fd;
                if (client_fd != listener && client_fd != new_fd)
                {
                    send(client_fd, buf, bytes, 0);
                }
            }
            bzero(buf, 1024);
        }
    }
    return NULL;
}
int main(void)
{
    printf("RUNNING.......\n");
    int newfd;                          
    struct sockaddr_storage remoteaddr; 
    socklen_t addrlen;

  
    fd_count = 0;
    int fd_size = 5;
    pfd = (pollfd *)malloc(sizeof *pfd * fd_size);

    listener = get_listener_socket();

    if (listener == -1)
    {
        fprintf(stderr, "error\n");
        exit(1);
    }

    pfd[0].fd = listener;
    pfd[0].events = POLLIN; 
    fd_count = 1;           

    for (;;)
    {
        int poll_count = poll(pfd, fd_count, -1);

        if (poll_count == -1)
        {
            perror("error in poll");
            exit(1);
        }

        for (int i = 0; i < fd_count; i++)
        {

            if (pfd[i].revents & POLLIN)
            { 

                if (pfd[i].fd == listener)
                {

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        add_to_pfds(&pfd, newfd, &fd_count, &fd_size);

                        printf("NEW CLIENT CONNECTED - ID : %d\n",newfd);
                        reactor* react = (reactor*)new reactor();
                        InstallHandler(react, &handler, &newfd);
                    }
                }
            }
        }
    }

    return 0;
}