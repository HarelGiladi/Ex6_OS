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






// answer to question in section 4
// you cant use this guard in order to protect strtok and make it thread safe
// because if a thread A enters this function with a string and delimiter he locks the function with guard
// and when he exits from the function he releases the guard.
// after that another thread B enters this function with a different string and different delimiter and updates the global
// variable, of course he locks the function but when he exits the function he releases the guard
// then thread A will call the strktok with NULL and his delimiter, but thread B already updated the global variable
// and thread A expects that the global variable to be the one he entered.
// so in conclusion you cant make strtok thread safe using this guard, because this guard prevents other threads to enter
// the function when another thread is in, but it doesnt prevent them from changing the global variable after the
// thread is done



//The C library function char *strtok(char *str, const char *delim) breaks string str into a series of tokens using the delimiter delim.
//we won't be able to use this function with this guard because many threads may be calling strtok with different strings.
// and it will only remember the last one and return the wrong result.


/* Q4 Answer:
 * When working with multiple threads, this guard cannot protect strtok
 * since multiple threads may be calling strtok with different string.
 * strtok will only keep track of the last string and eventually will
 * return the wrong result
 */