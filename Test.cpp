
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include "Queue.hpp"
#include "AO.hpp"
#include "reactor.hpp"



// #include "Mem_Imp.hpp"
// using namespace Ex5;
// using namespace std;


// #define PORT 6666

// bool precmp (const char *pre, const char *str)
// {
//     return strncmp(pre, str, strlen(pre)) == 0; 
// }

// int main()
// {
//     size_t SIZE  = 1024;
//     struct sockaddr_in server_address;
//     server_address.sin_family = AF_INET;
//     server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
//     server_address.sin_port = htons(PORT);
//     int sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (sock == -1)
//     {
//         perror("socket creation failed");
//         exit(1);
//     }
//     cout << "START" << endl;   

//     size_t serv_size = sizeof(server_address);
    
//     if (connect(sock,(struct sockaddr*)&server_address, serv_size)!= 0)
//     {
//         perror("socket connection failed");
//         exit(1);
//     }
//     cout << "connected..." << endl;
    
//     char * input;
//     input = (char*)Ex5::Mem_Imp::calloc(SIZE, sizeof(char));
//     recv(sock, input, 1024 ,0);

//     send(sock, "PUSH HI" , 8, 0);
//     recv(sock, input, 1024 ,0);
    
//     send(sock, "PUSH this is a test", 20, 0);
//     recv(sock, input, 1024 ,0);

//     send(sock, "TOP", 4, 0 );
//     recv(sock, input, 16 ,0);
//     assert(precmp(" this is a test", input));
    
//     send(sock, "POP" , 1024, 0);
//     recv(sock, input, 1024, 0);
//     assert(precmp(input, "POP COMPLETED"));

//     send(sock, "TOP", 1024, 0);
//     recv(sock, input, 1024, 0);
//     assert(precmp(" HI", input));
    
//     send(sock, "PUSH end", 1024, 0);
//     recv(sock, input, 1024 ,0);
    
//     send(sock, "TOP", 1024, 0);
//     recv(sock, input, 1024, 0);
//     if (precmp(" end",input)){cout << "TEST COMPLETED SUCCESSFULLY" << endl;}
// }


int main(){
    Queue *q1 = new Queue();
    Queue *q2 = new Queue();
    Queue *q3 = new Queue();

    active_object* ao_1 , *ao_2, *ao_3; 
    ao_1 = newAO(q1,NULL,NULL);
    ao_2 = newAO(q1,NULL,NULL);
    ao_3 = newAO(q1,NULL,NULL);

    reactor *r1 = new reactor();
    reactor *r2 = new reactor();
    reactor *r3 = new reactor();
    
    char ex6[] = {'e','x','6'};
    char ex6_2[] = {'e','x','6','2'};

    std::cout <<"started test on queue"<<std::endl;

    assert(q1->size == q2->size);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(q3->size == q2->size);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(q1->size == q3->size);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;

    q1->enQ(ex6);
    q1->enQ(ex6_2);
    q2->enQ(ex6);
    assert(q1->size == 2);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    q1->deQ();
    assert(q1->size ==1);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(q1->size == 1);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(q1->size == q2->size);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    q2->deQ();
    assert(q3->size == q2->size);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    std::cout <<"!TEST PASSED SECCSESFULLY!"<<std::endl;


    std::cout <<"test started on AO"<<std::endl;
    assert(ao_1->run == true);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(ao_2->run == true);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(ao_3->run == true);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(ao_3->run == ao_2->run);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(ao_2->run == ao_1->run);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(ao_1->run == ao_3->run);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    std::cout <<"!TEST PASSED SECCSESFULLY!"<<std::endl;



    std::cout <<"test started on reactor"<<std::endl;
    assert(r1->fd_count == r2->fd_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(r3->fd_count == r2->fd_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(r1->fd_count == r3->fd_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;

    assert(r1->func_count == r3->func_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(r1->func_count == r2->func_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(r2->func_count == r3->func_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;

    assert(r1->fd_count == r1->func_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    assert(r2->fd_count == r2->func_count);
    std::cout <<"!ASSERTION WAS A SECCSES!"<<std::endl;
    std::cout <<"!TEST PASSED SECCSESFULLY!"<<std::endl;
    std::cout <<"!ALL TESTS PASSED SECCSESFULLY!"<<std::endl;
        
}