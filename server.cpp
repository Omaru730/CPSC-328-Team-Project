#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define DEFAULT_PORT 50328

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


        /* Create TCP socket */
        int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd == -1) {
                perror("socket");
                exit(1);
        }


        /* Fill out the address form */
        struct sockaddr_in addr = {0};
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

                /*Send Greeting to client */
                send(client_fd, "HELLO\n", 6, 0);

                /* Hang up */
                close(client_fd);
                printf("Client disconnected\n");
        }


        close(listen_fd);
        return 0;

}
                    
