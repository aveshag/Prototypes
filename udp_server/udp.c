// client -> nc -u 127.0.0.1 5043
// server -> ./a.out 127.0.0.1 5043

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_LEN 1024

int main(int argc, char **argv)
{
    int listen_fd;
    struct sockaddr_in server_add, client_add;
    char buffer[BUFFER_LEN];
    printf("Server starting...\n");

    listen_fd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr(argv[1]);
    server_add.sin_port = htons(atoi(argv[2]));

    bind(listen_fd, (struct sockaddr *)&server_add, sizeof(server_add));
    printf("Server listening...\n");

    socklen_t client_add_size = sizeof(client_add);

    while (true)
    {
        recvfrom(listen_fd, buffer, BUFFER_LEN, 0, (struct sockaddr *)&client_add, &client_add_size);
        printf("Received data from client %s:%d\n", inet_ntoa(client_add.sin_addr), client_add.sin_port);
        printf("Received data: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}