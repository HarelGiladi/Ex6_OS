#include <iostream>
#include <pthread.h>
#include <string.h>
#include "Stack.hpp"
using namespace std;

pthread_mutex_t stack_mutex = PTHREAD_MUTEX_INITIALIZER;


Ex4::Stack::Stack()
{
    Ex4::Node *p = NULL;
    this->head = p;
}




// char *Ex4::Stack::TOP()
// {
//     pthread_mutex_lock(&stack_mutex);
//     if (this->head == NULL)
//     {
//         char *ans = (char *)Memory::malloc(25);
//         strcpy(ans, "DEBUG:Stack is empty.");
//         pthread_mutex_unlock(&stack_mutex);
//         return ans;
//     }
//     else
//     {
//         char *ans = (char *)Memory::malloc(strlen(this->head->data)+10);
//         strcpy(ans, "OUTPUT:");
//         strcat(ans, this->head->data);
//         pthread_mutex_unlock(&stack_mutex);
//         return ans;
//     }
// }
char * Ex4::Stack::TOP() 
{
    if (this->head == NULL) {
         char *output = (char *)Mem_Imp::malloc(25);
         strcpy(output, "DEBUG:STACK IS EMPTY");
        return output;
    } else {
        return head->data;
    }
}

// void Ex4::Stack::PUSH(char *val)
// {
//     pthread_mutex_lock(&stack_mutex);
//     if (strlen(val) > 1024)
//     {
//         pthread_mutex_unlock(&stack_mutex);
//         throw ::invalid_argument("DEBUG:string size has to be less then 1025.");
//     }
//     Ex4::Node *newnode = (Ex4::Node *)Memory::malloc(sizeof(Ex4::Node));
//     newnode->data = (char *)Memory::malloc(strlen(val) + 1);
//     strcpy(newnode->data, val);
//     newnode->next = this->head;
//     this->head = newnode;
//     pthread_mutex_unlock(&stack_mutex);
// }
void Ex4::Stack::PUSH(char * value) 
{
    Ex4::Node *newNode;
    //malloc implemented by us
    newNode = (Ex4::Node*)Mem_Imp::malloc(sizeof(Ex4::Node));
    memset(newNode,0,sizeof(struct Node));
    strcpy(newNode->data,value);
    if (head == NULL) {
        newNode->next = NULL;
    } else {
        newNode->next = head;
    }
    head = newNode;
}


// void Ex4::Stack::POP()
// {
//     pthread_mutex_lock(&stack_mutex);
//     if (this->head == NULL)
//         cout << "DEBUG:stack is empty." << endl;
//     else
//     {
//         Ex4::Node *temp = this->head;
//         this->head = this->head->next;
//         Memory::free(temp->data);
//         Memory::free(temp);
//     }
//     pthread_mutex_unlock(&stack_mutex);
// }

void Ex4::Stack::POP() 
{
    if (head == NULL) {
        //do nothing
    } else {
        Ex4::Node  *temp = head;
        head = head->next;
        free(temp);
    }
}