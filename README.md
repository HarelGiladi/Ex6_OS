# Ex4_OS


*Authors: Eilon Barashi & Harel Giladi*


Server & Client stack - using Malloc and free implementation

Implement a stack in a server which multiple clients can access and manipulate the stack the stack is synchronized.

How To Run:

Open terminal in the directory of the dowload files make sure they all in the same dir and type: "make all" This will compile server.cpp , client.cpp and Test.cpp.

After compiling:

To run the server : ./server.exe

To run the client : ./client.exe

To run the test : ./test.exe - A message "TEST COMPLETED SUCCSSEFULLY" will appear in the terminal if all tests have passed.
run ./server.exe before running the ./test.exe

Stack supported commands , from clients terminal :

PUSH "string" : will push the string to the top of the stack.

TOP : will display on the client's terminal the string that last entered to the stack (LIFO).

POP: will pop the last entered string to the stack from the stack.


Malloc & free implemntation: https://stackoverflow.com/questions/5422061/malloc-implementation