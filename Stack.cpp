#include <iostream>
#include <pthread.h>
#include <string.h>
#include "Stack.hpp"
using namespace std;



Ex4::Stack::Stack()
{
    Ex4::Node *p = NULL;
    this->head = p;
}

bool Ex4::Stack::IsEmpty()
{
    return this->head == NULL;
}

char* Ex4::Stack::TOP() 
{
    if (this->head == NULL) {
         char *output = (char *)Mem_Imp::malloc(25);
         strcpy(output, "DEBUG:STACK IS EMPTY");
        return output;
    } else {
        return head->data;
    }
}

void Ex4::Stack::POP()
{
    if (this->head == NULL)
        cout << "DEBUG:STACK IS EMPTY" << endl;
    else
    {
        Ex4::Node *temp = this->head;
        this->head = this->head->next;
        Mem_Imp::free(temp->data);
        Mem_Imp::free(temp);
    }
   
}

void Ex4::Stack::PUSH(char *val)
{
    Ex4::Node *newnode = (Ex4::Node *)Mem_Imp::malloc(sizeof(Ex4::Node));
    newnode->data = (char *)Mem_Imp::malloc(strlen(val));
    strcpy(newnode->data, val);
    newnode->next = this->head;
    this->head = newnode;
    
}


