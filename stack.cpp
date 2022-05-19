#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <string>

using namespace std;

typedef struct Nodes
{
    string data;
    struct Nodes* next;
}Node;

Node * head=NULL;

string TOP() {
    if (head == NULL) {
        //printf("STACK IS EMPTY\n");
        return "STACK IS EMPTY";
    } else {
        Node *temp = head;
        //printf("OUTPUT: %s\n", temp->data);
        return temp->data;
    }
}
void PUSH(string value) {
    Node *newNode;
    newNode = (struct Nodes*)calloc(1,sizeof(struct Nodes));
    newNode->data = value;
    if (head == NULL) {
        newNode->next = NULL;
    } else {
        newNode->next = head;
    }
    head = newNode;
}
string POP() {
    if (head == NULL) 
    {
        return "STACK IS EMPTY";
    } 
    else 
    {
        Node *temp = head;
        string tempData = head->data;
        head = head->next;
        free(temp);
        return tempData;
    }
}