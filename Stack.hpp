#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Mem_Imp.hpp"

namespace Ex5
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
            Node *head = NULL;

        public:
            int fd;
            char* curr_address = NULL;
            bool IsEmpty();
            int Init();
            //Stack();
            void PUSH(char* val);
            void POP();
            char* TOP();
            //char* memory();
    };
}