//https://www.geeksforgeeks.org/queue-linked-list-implementation/
//https://stackoverflow.com/questions/16522858/understanding-of-pthread-cond-wait-and-pthread-cond-signal

#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

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
    int fd;
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
        Node* temp = front;
        front = front->next;

        //  if (front == NULL)
        //     rear = NULL;
        
        // delete (temp);

        pthread_mutex_unlock(&(lock));
    }

    void set_fd(int new_fd) {
        pthread_mutex_lock(&(lock));
        this->fd = new_fd;
        pthread_mutex_unlock(&(lock));
    }

    int get_fd() {
        return this->fd;
    }
};

// Active Object - ex. 2

// struct active_object {
//     pthread_t t;
//     void (*func1)(void*);
//     void (*func2)(void*);
//     Queue* queue;
// };


// void* runAO (void* vao) {
    
//     active_object* ao = (active_object*)vao;
//     while (true) {
//         if (ao->queue->size > 0) {
//             char* tmp = (char*) malloc (sizeof(char));
//             tmp = (char*) ao->queue->front->data;        
//             ao->queue->deQ();

//             if (ao->func1 != NULL) {
//                 ao->func1(tmp);
//             }

//             if (ao->func2 != NULL) {
//                 ao->func2(tmp);
//             }
//         }

//     }
    
// }


// active_object* newAO (Queue* q, void (*f1)(void*), void (*f2)(void*)) {

//     active_object* ao = new active_object();
//     ao->func1 = f1;
//     ao->func2 = f2;
//     ao->queue = q;
//     pthread_create(&(ao->t), NULL, runAO, (void*)ao);
//     return ao;

// }

// void destroyAO (active_object* ao) {
//     ao->queue->destroyQ(); //clear the Queue
//     pthread_cancel(ao->t); //kill the thread
//     pthread_exit(NULL); //terminate the program
// }