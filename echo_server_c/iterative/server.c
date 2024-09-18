#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdbool.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#define SERVER_PORT 3000
#define LISTEN_QUEUE 8
#define LINE_LENGTH 4096


int main(int argc, char **argv) 
{
    int listen_fd, connect_fd, n;
    socklen_t client_len;
    char buffer[LINE_LENGTH];
    struct sockaddr_in server_add, client_add;

    // IP Type (Protocol family), Socket Type, Protocol
    // The protocol specifies a particular protocol to be used with the
    // socket.  Normally only a single protocol exists to support a
    // particular socket type within a given protocol family, in which
    // case protocol can be specified as 0.
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_add.sin_family = AF_INET;
    // htonl: Host byte order to Network byte order, l for long (32 Bits)
    // Network byte order is big endian
    // For big endian system it can be no-op

    // INADDR_ANY 0.0.0.0
    // INADDR_LOOPBACK 127.0.0.1
    server_add.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_add.sin_port = htons(SERVER_PORT);
    // sockaddr_in is protocol specific type
    bind(listen_fd, (struct sockaddr *) &server_add, sizeof(server_add));

    // LISTEN_QUEUE: maximum number of connections can queued
    listen(listen_fd, LISTEN_QUEUE);

    printf("Server running...\n");
    printf("Waiting for connections...\n");

    memset(buffer, 0, sizeof(buffer));
    while(true) {
        connect_fd = accept(listen_fd, (struct sockaddr *) &client_add, &client_len);

        char *ip = inet_ntoa(client_add.sin_addr);
        int port = client_add.sin_port;
        printf("\nReceived request from client %s:%d...", ip, port);
        printf("\nReceiving data from client...\n");
        while((n = recv(connect_fd, buffer, LINE_LENGTH, 0)) > 0)
        {
            printf("%s", buffer);
            // Echoing
            send(connect_fd, buffer, n, 0);
            memset(buffer, 0, sizeof(buffer));
        }

        if(n < 0)
        {
            perror("Error in reading data");
            exit(1);
        }

        close(connect_fd);
    }

    close(listen_fd);
    return 0;
}