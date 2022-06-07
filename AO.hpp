
#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "Queue.hpp"

struct active_object {
    pthread_t t;
    void (*func1)(void*);
    void (*func2)(void*);
    Queue* queue;
    bool run;
};


void* runAO (void* new_ao) {
    active_object* ao = (active_object*)new_ao;
    while (1) {
        if (ao->queue->size >0) {
            char* tmp = (char*) malloc (sizeof(char));
            tmp = (char*) ao->queue->front->data;        
            ao->queue->deQ();
            if (ao->func1 != NULL) {
                ao->func1(tmp);
            }

            if (ao->func2 != NULL) {
                ao->func2(tmp);
            }
        }

    }
    printf("active object terminated!\n");
}


active_object* newAO (Queue* q, void (*func1)(void*), void (*func2)(void*)) {

    active_object* ao = new active_object();    
    ao->func1 = func1;
    ao->func2 = func2;
    ao->queue = q;
    ao->run= true;
    pthread_create(&ao->t, NULL, runAO, (void*)ao);

    return ao;

}

void destroyAO (active_object* ao) {
    ao->queue->destroyQ(); 
    ao->run=false;
    pthread_cancel(ao->t); 
    pthread_exit(NULL); 
}