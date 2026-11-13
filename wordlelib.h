#ifndef WORDLELIB_H
#define WORDLELIB_H

// Send a message through a socket
// Returns 1 if successful and -1 if an error occurs
int sendMessage(int socketFD, const char message[]);

// Receive a message through a socket
// Returns 1 if successful, 0 if disconnected, -1 if error
int receiveMessage(int socketFD, char message[], int size);

#endif


//just declarations started, code to follow
