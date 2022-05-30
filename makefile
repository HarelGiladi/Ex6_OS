CC=g++
FLAGS=-Wall -pthread

all: Ex5

Ex5: server.o Mem_Imp.o Stack.o client.exe test.exe
	$(CC) $(FLAGS)  server.o Mem_Imp.o Stack.o -o server.exe

client.exe: client.cpp Mem_Imp.o
	$(CC) $(FLAGS) -o client.exe client.cpp Mem_Imp.o

server.o: server.cpp 
	$(CC) $(FLAGS) -c  server.cpp 

Stack.o: Stack.cpp Stack.hpp
	$(CC) $(FLAGS) -c Stack.cpp

Mem_Imp.o: Mem_Imp.cpp Mem_Imp.hpp
	$(CC) $(FLAGS) -c Mem_Imp.cpp

test.exe: 
		$(CC) Mem_Imp.o Test.cpp -o test.exe
		

PHONY:	all clean

clean:
	rm -f *.o .a *.exe *.txt Ex4
