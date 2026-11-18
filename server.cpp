#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <string>
#include "wordlelib.h"

#define DEFAULT_PORT 50300
#define MAX_MSG 128

const std::string WORDS[] = {"linux", "debug", "bytes", "stack", "while", "loops", "token", "ports"};

const int NUM_WORDS = 8;


void handleClient(int client_fd)
{
        char buf[MAX_MSG];

        /* Greet client */
        if (sendMessage(client_fd, "HELLO\n") == -1) {
                close(client_fd);
                return;
        }

        /* Continue talking until client quits or hangs up */
        while (receiveMessage(client_fd, buf, MAX_MSG) == 1) {
                std::string msg = buf;
                printf("Received: %s\n", buf);

                if (msg == "READY" || msg == "WORD") {
                        std::string reply = WORDS[rand() % NUM_WORDS] + "\n";
                        if (sendMessage(client_fd, reply.c_str()) == -1) break;
                        printf("Sent: %s", reply.c_str());
                }
                else if (msg == "QUIT" || msg == "BYE") {
                        break;
                }
                else if (sendMessage(client_fd, "ERROR\n") == -1) {
                        break;
                }
        }

        close(client_fd);
        printf("Client disconnected\n");
}


int main( int argc, char *argv[])
{
        int port;

        if (argc > 2) {
                fprintf(stderr, "Usage %s [port]\n", argv[0]);
                exit(1);
        }


        if (argc == 2)
                port = atoi(argv[1]);
        else
                port = DEFAULT_PORT;

        srand(time(NULL));


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
        while (1) {
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd == -1) {
                        perror("accept");
                        continue;
                }
                printf("Client connected\n");
                handleClient(client_fd);

        }


        close(listen_fd);
        return 0;
}

