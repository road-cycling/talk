#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>

#include "helper.hpp"

#include<curses.h>
#include <thread>

#define MAX_DATA 1
#define WAIT_QUEUE 10
#define num_rows 25
#define num_cols 80


// g++ -std=c++17 client.cpp helper.cpp -lcurses -ltermcap && ./a.out
// g++ -std=c++17 server.cpp helper.cpp -lcurses -ltermcap && ./a.out


int main() {

    int sock_fd;
    struct sockaddr_in client;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(-1);
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(10000);
    // client.sin_addr.s_addr = ;

    if (inet_pton(AF_INET, "127.0.0.1", &client.sin_addr) <= 0) {
        perror("inet_pton");
        exit(-1);
    }
    bzero(&client.sin_zero, 8);

    if ((connect(sock_fd, (struct sockaddr *)&client, sizeof(struct sockaddr_in))) == -1) {
        perror("Connect");
        exit(-1);
    }

   
    startup();
    std::thread first(sender, sock_fd);
    std::thread second(receiver, sock_fd);
    first.join();
    second.join();

    close(sock_fd);
    printf("Client is dead\n");


}