

# all: server client test

# server:
# 	clang server.cpp -pthread -o server
	
# client:
# 	clang client.cpp -o client 
# test:
# 	clang++ Test.cpp -o Test
# clean:
# 	rm -rf server
# 	rm -rf client
# 	rm -rf Test

CC=g++
FLAGS=-Wall -pthread

all: Ex4

Ex4: server.o Mem_Imp.o Stack.o client
	$(CC) $(FLAGS)  server.o Mem_Imp.o Stack.o -o server

server.o: server.cpp
	$(CC) $(FLAGS) -c server.cpp

MyStack.o: Stack.cpp Stack.hpp
	$(CC) $(FLAGS) -c Stack.cpp

MyMemory.o: Mem_Imp.cpp Mem_Imp.hpp
	$(CC) $(FLAGS) -c Mem_Imp.cpp

client: client.cpp
	$(CC) $(FLAGS) -o client client.cpp Mem_Imp.cpp 



clean:
	rm -f *.o .a server client Ex4 
