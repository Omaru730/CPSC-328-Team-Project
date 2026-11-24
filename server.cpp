/************************************************************/
/* Author: Omaru Jalloh                                     */
/* Major: Information Technology                            */
/* Creation Date: November 11, 2026                         */
/* Due Date: November 23, 2026                              */
/* Course: CSC328 020                                       */
/* Professor Name: Professor Walther                        */
/* Assignment: Group Project                                */
/* Filename: server.cpp                                     */
/* Purpose: This program is a concurrent, connection        */
/*          oriented TCP Wordle Server. It accepts an       */
/*          optional port number, then forks a child        */
/*          for each client. Each child sends HELLO and     */
/*          sends a random 5 letter word when the client    */
/*          sends READY or WORD, replies ERROR to invalid   */ 
/*          messages and ends when the client sends BYE,    */
/*          QUIT, or disconnects                            */
/*                                                          */
/************************************************************/
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <string>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "wordlelib.h"

#define DEFAULT_PORT 50400
#define MAX_MSG 128

const std::string WORDS[] = {"linux", "debug", "bytes", "stack", "while", "loops", "token", "ports"};

const int NUM_WORDS = 8;

/***********************************************************************/
/*                                                                     */
/* Function Name: handleClient                                         */
/* Description: Runs the Wordle protocol with a client. It will send   */
/*              a greeting(HELLO), replies to READY/WORD with a        */
/*              random word, replies ERROR to invalid messages and     */
/*              returns when the client sends BYE or QUIT or           */
/*              disconnects.                                           */
/*                                                                     */
/* Parameters: int client_fd: connected socket for this client - input */
/*             int clientId: unique id for this client - input         */
/* Return Value: void - none                                           */
/*                                                                     */
/***********************************************************************/
void handleClient(int client_fd, int clientId)
{
        char buf[MAX_MSG];

        /* Greet client */
        if (sendMessage(client_fd, "HELLO\n") == -1) {
                return;
        }

        /* Continue talking until client quits or hangs up */
        while (receiveMessage(client_fd, buf, MAX_MSG) == 1) {
                std::string msg = buf;
                printf("[Client %d] Received: %s\n", clientId, buf);

                if (msg == "READY" || msg == "WORD") {
                        std::string reply = WORDS[rand() % NUM_WORDS] + "\n";
                        if (sendMessage(client_fd, reply.c_str()) == -1) break;
                        printf("[Client %d] Sent: %s", clientId, reply.c_str());
                }
                else if (msg == "QUIT" || msg == "BYE") {
                        break;
                }
                else if (sendMessage(client_fd, "ERROR\n") == -1) {
                        break;
                }
        }

        printf("[Client %d] Client disconnected\n", clientId);
}

/***********************************************************************/
/*                                                                     */
/* Function Name: main                                                 */
/* Description: Validates the optional port argument, creates a TCP    */
/*              listening socket, and accepts clients in a loop. Forks */
/*              a child process for each client to run handleClient,   */
/*              while the parent keeps accepting more clients.         */
/*                                                                     */
/* Parameters: int argc: number of command line arguments - input      */
/*             char *argv[]: command line arguments: argv[1] is the    */
/*                           optional port number - input              */
/* Return Value: int - 0 on normal exit, Exits with 1 on a usage or    */
/*               socket setup error                                    */
/*                                                                     */
/***********************************************************************/
int main( int argc, char *argv[])
{
        int port;

        if (argc > 2) {
                fprintf(stderr, "Usage %s [port]\n", argv[0]);
                exit(1);
        }


        if (argc == 2) {
                char *end;
                long p = strtol(argv[1], &end, 10);
                if (*end != '\0' || p < 1024 || p > 65535) {
                        fprintf(stderr, "Invalid port '%s' (must be 1024 - 65535)\n", argv[1]);
                        fprintf(stderr, "Usage %s [port]\n", argv[0]);
                        exit(1);
                }
                port = int(p);
                printf("Port %d accepted\n", port);
        }
        else {
                port = DEFAULT_PORT;
        }


        /* Kernel reaps finished children to avoid zombies */
        signal(SIGCHLD, SIG_IGN);
        /*Sending to a closed client will return -1, no crash*/
        signal(SIGPIPE, SIG_IGN);



        /* Create TCP socket */
        int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd == -1) {
                perror("socket");
                exit(1);
        }

        /* Allow server to restart immediately on the same port */
        int yes = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));


        /* Fill out the address form */
        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);


        /* Bind the socket to the port */
        if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                perror("bind");
                exit(1);
        }


        /* Begin listening */
        if (listen(listen_fd, 5) == -1) {
                perror("listen");
                exit(1);
        }

        printf("Server listening on port %d\n", port);

        /* Answer client with accept */

        int clientCount = 0;

        while (1) {
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd == -1) {
                        if (errno == EINTR) continue;
                        perror("accept");
                        continue;
                }
                clientCount++;
                int clientId = clientCount;
                printf("[Client %d] connected\n", clientId);
                fflush(stdout);

                pid_t pid = fork();
                if (pid == -1) {
                        perror("fork");
                        close(client_fd);
                        continue;
                }
                



                if (pid == 0) {
                        /* Child */
                        close(listen_fd);
                        /* Child picks different words*/
                        srand(time(NULL) ^ getpid());
                        handleClient(client_fd, clientId);
                        close(client_fd);
                        exit(0);
                }

                /* Parent */
                close(client_fd);
        }
        close(listen_fd);
        return 0;

}

