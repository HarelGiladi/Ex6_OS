//https://www.geeksforgeeks.org/queue-linked-list-implementation/
//https://stackoverflow.com/questions/16522858/understanding-of-pthread-cond-wait-and-pthread-cond-signal

#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#pragma once
using namespace std;

 
struct Node {
    void* data;
    Node* next;
    
    Node(void* d) {
        data = d;
        next = NULL;
    }
};

struct Queue {
    Node *front, *rear;
    int *fd;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int size;

    Queue() {
        front = rear = NULL;
        if (pthread_mutex_init(&(lock), NULL) != 0) {
            perror("mutex");
        }
        this->cond = PTHREAD_COND_INITIALIZER;
        size = 0;
    }

    static Queue* createQ () {
        Queue* q = new Queue();
        return q;
    }
    
    void destroyQ() {
        pthread_mutex_lock(&(lock));
        while (front != NULL) {
            Node* temp = front;
            front = front->next;
            delete(temp);
        }
        pthread_mutex_unlock(&(lock));

    }
 
    void enQ(void* x) {

        pthread_mutex_lock(&(lock));
        this->size++;

        Node* temp = new Node(x);
       
        if (rear == NULL) {
            front = rear = temp;
        }
 
        rear->next = temp;
        rear = temp;
        
        pthread_cond_signal(&(cond)); 
        pthread_mutex_unlock(&(lock));
    }

    
 
    void deQ()
    {
        pthread_mutex_lock(&(lock));
        
        if (this->size == 0) {
            rear = NULL;
            pthread_cond_wait(&(cond), &(lock));
        }
      
        this->size--;
        front = front->next;


        pthread_mutex_unlock(&(lock));
    }

    void set_fd(int new_fd) {
        pthread_mutex_lock(&(lock));
        int* pfd = (int*) malloc (sizeof(int));
        *pfd = new_fd;
        this->fd = pfd;
        pthread_mutex_unlock(&(lock));
    }

    int* get_fd() {
        return this->fd;
    }
};

