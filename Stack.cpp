#include <iostream>
#include <pthread.h>
#include <string.h>
#include "Stack.hpp"
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;
//int fd;
struct flock locker;

bool Ex5::Stack::IsEmpty()
{
    //locker.l_type = F_WRLCK;
    //fcntl(fd, F_SETLKW, &locker);
    return this->head == NULL;
   // locker.l_type = F_UNLCK;
    ///fcntl(fd, F_SETLKW, &locker);
}

char* Ex5::Stack::TOP() 
{
    ///locker.l_type = F_WRLCK;
    //fcntl(fd, F_SETLKW, &locker);
    if (this->head == NULL) {
         char *output = (char *)Mem_Imp::malloc(25);
         strcpy(output, "DEBUG:STACK IS EMPTY");
        return output;
    } else {
        return this->head->data;
    }
    //locker.l_type = F_UNLCK;
    //fcntl(fd, F_SETLKW, &locker);

}

void Ex5::Stack::POP()
{    
   // locker.l_type = F_WRLCK;
   // fcntl(fd, F_SETLKW, &locker);
   
    if (this->head == NULL)
        cout << "DEBUG:STACK IS EMPTY" << endl;
    else
    {
        // Ex5::Node *temp = (*stack)->head;
        this->head = this->head->next;
        // Mem_Imp::free(temp->data);
        // Mem_Imp::free(temp);
    }
   // locker.l_type = F_UNLCK;
   // fcntl(fd, F_SETLKW, &locker);  
   
}

void Ex5::Stack::PUSH(char *val)
{
   // locker.l_type = F_WRLCK;
    //fcntl(fd, F_SETLKW, &locker);
    Ex5::Node *newnode =(Ex5::Node*)this->curr_address;
    newnode->data = (char *)Mem_Imp::malloc(strlen(val));
    strcpy(newnode->data, val);
    newnode->next = this->head;
    this->head = newnode;
   // locker.l_type = F_UNLCK;
    //fcntl(fd, F_SETLKW, &locker);
}

