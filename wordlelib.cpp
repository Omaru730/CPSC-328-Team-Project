/*******************************************************************************/
/* Author:            Jason Vaux                                               */
/* Date Updated:      November 23, 2026                                        */
/* Due Date:          November 23, 2026                                        */
/* Course:            CPSC 328                                                 */
/* Professor Name:    Mr. Walther                                              */
/* Assignment:        Team Network Program Design                              */
/* Filename:          wordlelib.cpp                                            */
/* Purpose:           Implement the shared TCP message functions.              */
/*******************************************************************************/


#include "wordlelib.h"
#include <sys/socket.h>
#include <string.h>

/*****************************************************************************/
/* Function name:    sendMessage                                             */
/* Description:      Send a complete newline-terminated message over a TCP   */
/*                   socket. Keep sending until all bytes are sent.          */
/* Parameters:       socketFD - connected socket descriptor                  */
/*                   message - text ending with a newline (\n)               */
/* Return Value:     1 on success; -1 on error or invalid message            */
/*****************************************************************************/

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

/*****************************************************************************/
/* Function name:    receiveMessage                                          */
/* Description:      Receive characters until a newline is found. Remove     */
/*                   the newline and terminate the result with '\0'.         */
/* Parameters:       socketFD - connected socket descriptor                  */
/*                   message - character array for the received text         */
/*                   size - total size of the character array                */
/* Return Value:     1 on success; 0 on disconnect; -1 on error or           */
/*                   message too long                                        */
/*****************************************************************************/
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

