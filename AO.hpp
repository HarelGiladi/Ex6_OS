#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "Queue.hpp"

struct active_object {
    pthread_t *t;
    void* (*func1)(void*);
    void* (*func2)(void*);
    Queue* queue;
    bool run;
};


void* runAO (void* new_ao) {
    std::cout<<"run"<<std::endl;
    active_object* ao = (active_object*)new_ao;
    while (1) {
        if (ao->queue->front != nullptr) {
            std::cout<<"in"<<std::endl;
            char* tmp = (char*) malloc (sizeof(char));
            tmp = (char*) ao->queue->front->data;        
            ao->queue->deQ();
            std::cout<<tmp<<std::endl;
            if (ao->func1 != NULL) {
                ao->func1(tmp);
            }

            if (ao->func2 != NULL) {
                ao->func2(tmp);
            }
        }

    }
    free(ao->t);
    free(ao);
    printf("active object terminated!\n");
    return new_ao;
}


active_object* newAO (Queue* q, void* (*func1)(void*), void* (*func2)(void*)) {

    active_object *ao = (active_object*)malloc(sizeof(active_object));
    ao->func1 = func1;
    ao->func2 = func2;
    ao->queue = q;
    ao->run= true;
    ao->t = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_create(ao->t, NULL, runAO, (void*)ao);

    return ao;

}

void destroyAO (active_object* ao) {
    ao->queue->destroyQ(); 
    ao->run=false;
    free(ao->t);
    free(ao);
    pthread_exit(NULL); 
}