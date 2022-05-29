#include <iostream>
#include <pthread.h>
#include <string.h>
#include "Stack.hpp"
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;

struct flock locker;

// void Ex5::Stack::init_Stack(Stack **stack)
// {
//     //Ex5::Node *p = NULL;
//     (*stack)->head = NULL;
//     (*stack)->curr_address = (*char)+sizeof(Ex5::Stack);
// }

// bool Ex5::Stack::IsEmpty(Stack **stack)
// {
//     return (*stack)->head == NULL;
// }

// char* Ex5::Stack::TOP(Stack **stack) 
// {
//     if ((*stack)->head == NULL) {
//          char *output = (char *)Mem_Imp::malloc(25);
//          strcpy(output, "DEBUG:STACK IS EMPTY");
//         return output;
//     } else {
//         return (*stack)->head->data;
//     }

// }

// void Ex5::Stack::POP(Stack **stack)
// {
//     if ((*stack)->head == NULL)
//         cout << "DEBUG:STACK IS EMPTY" << endl;
//     else
//     {
//         // Ex5::Node *temp = (*stack)->head;
//         (*stack)->curr_address -= sizeof(Ex5::Node);
//         (*stack)->head = (*stack)->head->next;
//         // Mem_Imp::free(temp->data);
//         // Mem_Imp::free(temp);
//     }
   
// }

// void Ex5::Stack::PUSH(Stack **stack,char *val)
// {
//     (*stack)->curr_address += sizeof(Ex5::Node);
//     Ex5::Node *newnode =(Ex5::Node*)(*stack)->curr_address;
//     newnode->data = (char *)Mem_Imp::malloc(strlen(val));
//     strcpy(newnode->data, val);
//     newnode->next = (*stack)->head;
//     (*stack)->head = newnode;
    
// }

// char* Ex5::Stack::memory(Stack **stack)
// {
//     char *memory = (char*)mmap(NULL, sizeof(Ex5::Node)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
//     if ((void*)-1 == memory) {
//         printf("ERROR: could not map memory");
//         exit(1);
//     }
//     (*stack)->curr_address = memory;
//     return memory;
// }



bool Ex5::Stack::IsEmpty()
{
    return this->head == NULL;
}

char* Ex5::Stack::TOP() 
{
    if (this->head == NULL) {
         char *output = (char *)Mem_Imp::malloc(25);
         strcpy(output, "DEBUG:STACK IS EMPTY");
        return output;
    } else {
        return this->head->data;
    }
   

}

void Ex5::Stack::POP()
{
   
    if (this->head == NULL)
        cout << "DEBUG:STACK IS EMPTY" << endl;
    else
    {
        // Ex5::Node *temp = (*stack)->head;
        this->curr_address -= sizeof(Ex5::Node);
        this->head = this->head->next;
        // Mem_Imp::free(temp->data);
        // Mem_Imp::free(temp);
    }
   
   
}

void Ex5::Stack::PUSH(char *val)
{
   
    this->curr_address += sizeof(Ex5::Node);
    Ex5::Node *newnode =(Ex5::Node*)this->curr_address;
    newnode->data = (char *)Mem_Imp::malloc(strlen(val));
    strcpy(newnode->data, val);
    newnode->next = this->head;
    this->head = newnode;

}

// char* Ex5::Stack::memory(Stack **stack)
// {
//     char *memory = (char*)mmap(NULL, sizeof(Ex5::Node)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
//     if ((void*)-1 == memory) {
//         printf("ERROR: could not map memory");
//         exit(1);
//     }
//     (*stack)->curr_address = memory;
//     return memory;
// }