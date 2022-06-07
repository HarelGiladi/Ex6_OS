CC=g++
FLAGS=-Wall -pthread

all: Ex6

Ex6: singleton_test.exe guard_test.exe server.exe client.exe select_client.exe pollserver.exe

# running programs
server.exe: server.o Queue.hpp 
	$(CC) $(FLAGS) server.o -o server.exe 
client.exe: client.o
	$(CC) $(FLAGS) -o client.exe client.o
pollserver.exe: pollserver.o reactor.hpp
	$(CC) $(FLAGS) pollserver.o -o pollserver.exe 
select_client.exe: select_client.o reactor.hpp
	$(CC) $(FLAGS) select_client.o -o select_client.exe
singleton_test.exe: singleton.o
	$(CC) $(FLAGS) singleton.o -o singleton_test.exe
guard_test.exe: guard.o
	$(CC) $(FLAGS) guard.o -o guard_test.exe

#object files before linker
server.o: server.cpp 
	$(CC) $(FLAGS) -c server.cpp
client.o: client.cpp 
	$(CC) $(FLAGS) -c client.cpp
pollserver.o: pollserver.cpp 
	$(CC) $(FLAGS) -c pollserver.cpp
select_client.o: select_client.cpp 
	$(CC) $(FLAGS) -c select_client.cpp
singleton.o: singleton.cpp
	$(CC) $(FLAGS) -c singleton.cpp
guard.o: guard.cpp
	$(CC) $(FLAGS) -c guard.cpp


.PHONY: clean all

clean:
	rm -rf *.o  *.exe