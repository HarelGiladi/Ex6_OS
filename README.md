# Ex4_OS


Server & Client stack - using Malloc and free implementation


Authors: Eilon Barashi & Harel Giladi


Implement a stack in a server which multiple clients can access and manipulate the stack the stack is synchronized.

How To Run:

Open terminal in the directory of the dowload files make sure they all in the same dir and type: "make all" This will compile server.cpp , client.cpp and Test.cpp.

After compiling:

To run the server : ./server

To run the client : ./client

To run the test : ./Test - a " the test has been successfully completed" message will appear in the terminal if all tests have passed.

Stack supported commands , from clients terminal :

PUSH "string" : will push the string to the top of the stack.

TOP : will display on the client's terminal the string that last entered to the stack (LIFO).

POP: will pop the last entered string to the stack from the stack.


Malloc & free implemntation: https://stackoverflow.com/questions/5422061/malloc-implementation