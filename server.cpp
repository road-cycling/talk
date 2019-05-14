#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>

#include "helper.hpp"

#define MAX_DATA 1
#define WAIT_QUEUE 10
#define num_rows 25
#define num_cols 80


int main() {

    int sock;
    struct sockaddr_in server, client;
    char msg[] = "Hello World!";

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket: ");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(10000);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&server.sin_zero, 8);

    socklen_t len = sizeof(struct sockaddr_in);
    if((bind(sock, (struct sockaddr *)&server , len)) == -1) {
        perror("Bind");
        exit(-1);
    }

    if ((listen(sock, WAIT_QUEUE)) == -1) {
        perror("listen");
        exit(-1);
    }

    while (1) {

        int cli_fd;

        cli_fd = accept(sock, (struct sockaddr *)&client, &len);
        
        if ( cli_fd == -1 ) {
            perror("accept");
            exit(-1);
        }

        if ( fork() == 0 ) {
            close(sock);
            startup();
            std::thread first(sender, cli_fd);
            std::thread second(receiver, cli_fd);
            first.join();
            second.join();
        }
        close(cli_fd);
        // printf("New Client connected from port no %d and IP %s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));

        
        // close(cli_fd);

    }

    return 0;
}