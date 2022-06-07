#ifndef GUARD_HH
#define GUARD_HH

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <cassert>


class Guard{
private:
    pthread_mutex_t* locker;
public:
    Guard(pthread_mutex_t* locker){
        this->locker = locker;
        pthread_mutex_lock(this->locker);
    }
    ~Guard(){
        pthread_mutex_unlock(this->locker);
    }
};
#endif 


int counter;
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;

void* increasment_by_666(void* arg){
    Guard guarder(&locker);
    for(int i = 0; i < 666;i++){counter++;}
    return NULL;
}
int main(){
    pthread_t t1,t2,t3;
    std::cout <<  " starts testing:\n" <<std::endl;
    pthread_create(&t1, NULL, increasment_by_666, NULL);
    pthread_create(&t2, NULL, increasment_by_666, NULL);
    pthread_join(t1, NULL);
    assert(1332==counter);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n"<<std::endl;
    pthread_join(t2, NULL);
    assert(counter == 1332);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n" <<std::endl;


    pthread_create(&t3, NULL, increasment_by_666, NULL);
    assert(counter == 1332);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n" <<std::endl;
    pthread_join(t3, NULL);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n" <<std::endl;
    assert(counter == 1998);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n " <<std::endl;
    assert(1998==counter);
    std::cout <<  " !ASSERTION WAS A SUCCESS!\n " <<std::endl;
    std::cout <<  " !TEST PASSED SECCESSFULLY!\n" <<std::endl;

    return 0;
}





/*
 short answer we can't use gurad for protecting strtok.
 ---------------------------------------------------------
 more in depth, beacuse it will lock strtok while a thread is using it
 but after it is done another thread can use the funcation and change the global
 locker and that will cause the func to "remmeber" only the last thread who call her.
 to sum up it wont protect the global locker which is a serious problam.
*/