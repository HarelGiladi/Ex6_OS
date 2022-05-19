#include "stack.cpp"
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

int main(){
    PUSH("hey");
    assert(TOP() == "hey");
    PUSH("hello");
    POP();
    assert(TOP() == "hey");
    POP();
    assert(TOP() == "STACK IS EMPTY");
    POP();
    assert(TOP() == "STACK IS EMPTY");
    char c = 97;
    for (size_t i = 0; i < 20; i++)
    {
        string check;
        check.push_back(c);
        PUSH(check);
        c++;
    }
    assert(TOP() == "t");
    for (size_t i = 0; i < 5; i++)
    {
        POP();
    }

    assert(TOP() == "o");

    for (size_t i = 0; i < 100; i++)
    {
        POP();
    }

    assert(TOP() == "STACK IS EMPTY");

    cout << "Test has been done successfully" << endl;

}