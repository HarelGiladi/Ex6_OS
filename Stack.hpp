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
            char* curr_address;

        public:
            bool IsEmpty();
            Stack();
            static void PUSH(Stack *stack, char* val);
            static void POP(Stack *stack);
            static char* TOP(Stack *stack);
            static char* memory(Stack* stack);
    };
}