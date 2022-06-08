CC=g++
FLAGS=-Wall -pthread

all: Ex6

Ex6: singleton_test.exe guard_test.exe server.exe client.exe select_client.exe pollserver.exe test.exe

# running programs
server.exe: server.o Queue.hpp AO.hpp
	$(CC) $(FLAGS) server.o -o server.exe 
client.exe: client.o AO.hpp
	$(CC) $(FLAGS) -o client.exe client.o
pollserver.exe: pollserver.o reactor.hpp
	$(CC) $(FLAGS) pollserver.o -o pollserver.exe 
select_client.exe: selectclient.o reactor.hpp
	$(CC) $(FLAGS) selectclient.o -o select_client.exe
singleton_test.exe: singleton.o
	$(CC) $(FLAGS) singleton.o -o singleton_test.exe
guard_test.exe: guard.o
	$(CC) $(FLAGS) guard.o -o guard_test.exe
test.exe: Test.o reactor.hpp Queue.hpp AO.hpp
	$(CC) $(FLAGS) Test.o -o test.exe

#object files before linker
server.o: server.cpp 
	$(CC) $(FLAGS) -c server.cpp
client.o: client.cpp 
	$(CC) $(FLAGS) -c client.cpp
pollserver.o: pollserver.cpp 
	$(CC) $(FLAGS) -c pollserver.cpp
selectclient.o: selectclient.cpp 
	$(CC) $(FLAGS) -c selectclient.cpp
singleton.o: singleton.cpp
	$(CC) $(FLAGS) -c singleton.cpp
guard.o: guard.cpp
	$(CC) $(FLAGS) -c guard.cpp
Test.o: Test.cpp
	$(CC) $(FLAGS) -c Test.cpp


.PHONY: clean all

clean:
	rm -rf *.o  *.exe