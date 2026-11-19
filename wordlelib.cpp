
#include "wordlelib.h"
#include <sys/socket.h>
#include <string.h>

// Send a complete message through a socket
int sendMessage(int socketFD, const char message[])
{
    int length = strlen(message);
    int totalSent = 0;
    int n;

    // Make sure the message ends with a newline
    if (length == 0 || message[length - 1] != '\n') {
        return -1;
    }

    // Keep sending until the message is complete
    while (totalSent < length) {

        n = send(socketFD, &message[totalSent],
                 length - totalSent, 0);

        if (n <= 0) {
            return -1;
        }

        totalSent = totalSent + n;
    }

    return 1;
}


// Receive a complete message through a socket
int receiveMessage(int socketFD, char message[], int size)
{
    char ch;
    int i = 0;
    int n;
    int tooLong = 0;

    if (size < 2) {
        return -1;
    }

    // Read one character at a time
    while (1) {

        n = recv(socketFD, &ch, 1, 0);

        if (n == 0) {
            return 0;  // Connection closed
        }

        if (n < 0) {
            return -1;  // Error receiving
        }

        // Stop when the newline is received
        if (ch == '\n') {
            message[i] = '\0';

            if (tooLong == 1) {
                return -1;
            }

            return 1;
        }

        // Save the character if there is room
        if (i < size - 1) {
            message[i] = ch;
            i++;
        }
        else {
            tooLong = 1;
        }
    }
}

