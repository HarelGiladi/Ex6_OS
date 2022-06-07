CC=g++
 FLAGS=-Wall -pthread

all: Ex5

Ex5: server.o client.exe test.exe
	$(CC) $(FLAGS)  server.o  -o server.exe

client.exe: client.cpp Queue.hpp AO.hpp
	$(CC) $(FLAGS) -o client.exe client.cpp 

server.o: server.cpp
	$(CC) $(FLAGS) -c  server.cpp 

test.exe:

PHONY:	all clean

clean:
	rm -f *.o .a *.exe *.txt Ex5
