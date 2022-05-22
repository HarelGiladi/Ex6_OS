#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Mem_Imp.hpp"

namespace Ex4
{
    class Node
    {
    public:
        char* data;
        Node *next;
    };

    class Stack
    {

    private:
        Node *head;

    public:
        bool IsEmpty();
        Stack();
        void PUSH(char* val);
        void POP();
        char* TOP();
    };
}