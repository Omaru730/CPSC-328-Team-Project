/*******************************************************************************/
/* Author:            Jason Vaux                                               */
/* Date Updated:      November 23, 2026                                        */
/* Due Date:          November 23, 2026                                        */
/* Course:            CPSC 328                                                 */
/* Professor Name:    Mr. Walther                                              */
/* Assignment:        TEAM Program Application (Wordle Team Project)           */
/* Filename:          wordlelib.h                                              */
/* Purpose:           Declare the shared TCP message functions.                */
/*******************************************************************************/


#ifndef WORDLELIB_H
#define WORDLELIB_H


/*****************************************************************************/
/* Function name:    sendMessage                                             */
/* Description:      Send a complete newline-terminated message over a TCP  */
/*                   socket. Keep sending until all bytes are sent.          */
/* Parameters:       socketFD - connected socket descriptor                 */
/*                   message - text ending with a newline (\n)              */
/* Return Value:     1 on success; -1 on error or invalid message           */
/*****************************************************************************/
int sendMessage(int socketFD, const char message[]);

/*****************************************************************************/
/* Function name:    receiveMessage                                          */
/* Description:      Receive characters until a newline is found. Remove   */
/*                   the newline and terminate the result with '\0'.        */
/* Parameters:       socketFD - connected socket descriptor                 */
/*                   message - character array for the received text        */
/*                   size - total size of the character array               */
/* Return Value:     1 on success; 0 on disconnect; -1 on error or          */
/*                   message too long                                        */
/*****************************************************************************/
int receiveMessage(int socketFD, char message[], int size);

#endif


