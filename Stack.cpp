#include <iostream>
#include <pthread.h>
#include <string.h>
#include "Stack.hpp"
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;

struct flock locker;

Ex4::Stack::Stack()
{
    Ex4::Node *p = NULL;
    this->head = p;
    this->curr_address = NULL;
}

bool Ex4::Stack::IsEmpty()
{
    return this->head == NULL;
}

char* Ex4::Stack::TOP(Stack *stack) 
{
    if (stack->head == NULL) {
         char *output = (char *)Mem_Imp::malloc(25);
         strcpy(output, "DEBUG:STACK IS EMPTY");
        return output;
    } else {
        return stack->head->data;
    }

}

void Ex4::Stack::POP(Stack *stack)
{
    if (stack->head == NULL)
        cout << "DEBUG:STACK IS EMPTY" << endl;
    else
    {
        Ex4::Node *temp = stack->head;
        stack->head = stack->head->next;
        stack->curr_address -= sizeof(Ex4::Node);
        Mem_Imp::free(temp->data);
        Mem_Imp::free(temp);
    }
   
}

void Ex4::Stack::PUSH(Stack *stack,char *val)
{
    stack->curr_address += sizeof(Ex4::Node);
    Ex4::Node *newnode =(Ex4::Node*)stack->curr_address;
    // newnode->data = (char *)Mem_Imp::malloc(strlen(val));
    strcpy(newnode->data, val);
    newnode->next = stack->head;
    stack->head = newnode;
    
}

char* Ex4::Stack::memory(Stack *stack)
{
    char *memory = (char*)mmap(NULL, sizeof(Ex4::Node)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if ((void*)-1 == memory) {
        printf("ERROR: could not map memory");
        exit(1);
    }
    stack->curr_address = memory;
    return memory;
}


