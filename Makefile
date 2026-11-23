# CPSC 328 Wordle Team Project

#tells make file that goal is to build server and client
all: client server

#compile into object file ( "-c" without linking)
wordlelib.o: wordlelib.cpp wordlelib.h
	g++ -Wall -Wextra -c wordlelib.cpp -o wordlelib.o

#makes static library
libwordle.a: wordlelib.o
	ar rcs libwordle.a wordlelib.o

#compile and link client to static library
client: Client.cpp wordlelib.h libwordle.a
	g++ -Wall -Wextra Client.cpp libwordle.a -o client

#compile and link server to static library
server: server.cpp wordlelib.h libwordle.a
	g++ -Wall -Wextra server.cpp libwordle.a -o server

clean:
	rm -f wordlelib.o libwordle.a client server
