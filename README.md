# CPSC-328-Team-Project

##Project Description
This is a CPSC 328 team project to create a Wordle-style game using a TCP client and server in C++. The server will provide words and support multiple clients. The client will handle the player's guesses and display game feedback.

### Project Lead:
- Keith Groves.

### Team Responsibilities: 
- Keith Groves - client.
- Omaru Jalloh - server.
- Jason Vaux - library.

## Files & File	Purpose
- Client.cpp	Client program and Wordle gameplay
- server.cpp	TCP server
- wordlelib.h	Function declarations for the common library.
- wordlelib.cpp	Implementations of the library's sending and receiving functions.

We chose a static library (libwordle.a). It is built from wordlelib.cpp and linked into programs during compilation. This avoids requiring a separate shared-library file when running the executables. 


Both the client and server will use these functions from the static library:
- int sendMessage(int socketFD, const char message[]):
##### Sends a newline-terminated message through a connected TCP socket. It continues sending until all bytes have been sent or an error occurs. Returns 1 on success and -1 on error or invalid input.
- int receiveMessage(int socketFD, char message[], int size):
##### Reads one character at a time until a newline is received. It removes the newline and adds a string terminator ('\0'). Returns 1 when a complete message is received, 0 if the connection closes, and -1 on error or if the message is too long.



## Build the Static Library (Linux / ACAD)
From the project folder, run:
- g++ -Wall -Wextra -c wordlelib.cpp -o wordlelib.o
- ar rcs libwordle.a wordlelib.o



## Link the Library with the Programs
After Client.cpp and server.cpp include "wordlelib.h" and actually call the library functions, they can be compiled with:
- g++ -Wall -Wextra Client.cpp libwordle.a -o client
- g++ -Wall -Wextra server.cpp libwordle.a -o server


## Build Using the Makefile or Manual Instructions
You can compile the program manually using the steps below or use the included Makefile to simplify compiling the library, client, and server on Linux/ACAD.

Client Execution:
After compiling the client, run ./client with a valid server address. Optionally, you can also run a custom port number:
- ./client 127.0.0.2
- ./client 127.0.0.2 50300

Server Execution:
After compiling the server, run ./server. Optionally, you can also run a custom port number:
- ./server
- ./server 50300

**Communication Protocol**:
The application uses TCP (AF_INET, SOCK_STREAM). Messages are plain text terminated by a newline (\n).
- Sender	Example message	Meaning
- Server	HELLO\n	Greeting when a client connects.
- Client	READY\n	Request a Wordle word.
- Server	WORD black\n	Send a word (currently black in the test server).
- Client	BYE\n	End the session.

## References

- GitHub Docs. Basic writing and formatting syntax.
  https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
