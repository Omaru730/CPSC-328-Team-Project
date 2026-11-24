# CPSC-328-Team-Project

## Project Description
This is a CPSC 328 team project to create a Wordle-style game using a TCP client and server in C++. The server will provide words and support multiple clients. The client will handle the player's guesses and display game feedback.

### Project Lead:
- Keith Groves.

### Team Responsibilities: 
- Keith Groves - client.
- Omaru Jalloh - server.
- Jason Vaux - library.

## Files & File	Purpose
- `Client.cpp`	Client program and Wordle gameplay.
- `server.cpp`	TCP server.
- `wordlelib.h`	Function declarations for the common library.
- `wordlelib.cpp`	Implementations of the library's sending and receiving functions.
- `Makefile` Compiles the static library, client, and server.
- `README.md` Project documentation and instructions.

We chose a static library (libwordle.a). It is built from wordlelib.cpp and linked into programs during compilation. This avoids requiring a separate shared-library file when running the executables. 


## Static Library Functions
#### int sendMessage(int socketFD, const char message[])
###### Sends a newline-terminated message through a connected TCP socket. It continues sending until all bytes have been sent or an error occurs. Returns 1 on success and -1 on error or invalid input.
#### int receiveMessage(int socketFD, char message[], int size)
###### Reads one character at a time until a newline is received. It removes the newline and adds a string terminator ('\0'). Returns 1 when a complete message is received, 0 if the connection closes, and -1 on error or if the message is too long.




## Compiling the Program (Linux / ACAD)

The client, server, and static library can be compiled using the Makefile or manually using the commands below.

### Option 1: Using the Makefile

From the project folder, enter:

`make`

This compiles the static library, client, and server.

To remove the compiled files, enter:

`make clean`

### Option 2: Manual Compilation

**Step 1: Build the static library**

`g++ -Wall -Wextra -c wordlelib.cpp -o wordlelib.o`

`ar rcs libwordle.a wordlelib.o`

**Step 2: Compile the client and server**

`g++ -Wall -Wextra Client.cpp libwordle.a -o client`

`g++ -Wall -Wextra server.cpp libwordle.a -o server`

Both programs use the functions declared in `wordlelib.h` and implemented in `wordlelib.cpp`.

---

## Running the Program

### Server Execution

Start the server first. The default port is `50400`.

**Default port:**

`./server`

**Custom port:**

`./server 50300`

### Client Execution

Open another terminal to run the client. Enter the server's IP address or hostname.

**Default port:**

`./client 127.0.0.1`

**Custom port:**

`./client 127.0.0.1 50300`

**Using localhost:**

`./client localhost`

The client and server must use the same port number. The address `127.0.0.1` is used when the client and server are running on the same computer.


## Communication Protocol

The client and server communicate using TCP (`AF_INET`, `SOCK_STREAM`). Messages are sent as plain text and end with a newline character (`\n`).

The `receiveMessage()` function reads one character at a time until it receives a newline. This allows the program to know when the complete message has been received.


### Protocol Messages

| Sender | Message | Purpose |
|---|---|---|
| Server | `HELLO\n` | Greets the client when connected. |
| Client | `READY\n` or `WORD\n` | Requests a random word. |
| Server | `linux\n` | Example of a random five-letter word. |
| Client | `QUIT\n` or `BYE\n` | Ends the connection. |
| Server | `ERROR\n` | Responds to an unsupported message. |


### How the Protocol Works

1. The client connects to the server.
2. The server sends `HELLO`.
3. The client responds with `READY`.
4. The server selects and sends a random five-letter word.
5. The client allows the player six attempts to guess the word.
6. After winning or losing, the player can choose to play again.
7. If the player chooses to play again, the client sends another `READY` message.
8. If the player chooses to quit, the client sends `QUIT` and closes the connection.

## Team Collaboration

Our team divided the project into three main responsibilities. Keith Groves worked on the client, Omaru Jalloh worked on the server, and Jason Vaux worked on the static library, Makefile, and README.

We used GitHub to manage the project files and worked on separate branches for our assigned sections. The files were combined into the main branch so the client, server, and library could be compiled and tested together. Additionally, we used Discord for constant messaging and had several meetings over voice channels.

## Known Application Issues

The client does not currently display the remaining unguessed letters.


## References

- GitHub Docs. Basic writing and formatting syntax.
  https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
