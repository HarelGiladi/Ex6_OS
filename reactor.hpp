#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define MAX_NUM 66

struct reactor {
    
    pthread_t t;
    pthread_mutex_t lock;
    struct pollfd pfd[MAX_NUM];
    int fd_count = 0;
    int func_count = 0; 
    void* (*funcs[MAX_NUM])(void*);
    int fd;


};

void* newReactor() {

    reactor* r = new reactor();
    return r;

}

void* InstallHandler(reactor* r, void* (f)(void*), int *fd) {
        r->pfd[r->fd_count].fd = *fd;
        r->funcs[r->func_count] = f;
        r->pfd->events = POLLIN;
        r->fd_count++;
        r->func_count++;
        r->fd = *fd;
        pthread_create(&(r->t), NULL, f, r);
    return r;
}

void* RemoveHandler(reactor* r, int fd) {
    for (int i =0; i < r->fd_count; i++) {
        if (r->pfd[i].fd == fd) {
            r->pfd[i].fd = -1;
            r->pfd[i].events = 0;
            r->funcs[i] = NULL;
            r->fd_count--;
            r->func_count--;
        }
    }
    close(fd);
    return r;
}