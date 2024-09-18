#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define LINE_LENGTH 4096

int main(int argc, char **argv)
{
    int sock_fd;
    struct sockaddr_in server_add;
    char send_buffer[LINE_LENGTH], recev_buffer[LINE_LENGTH];

    if (argc != 3) {
        perror("Usage: tcpclient <IP address of the server> <Port of server>");
        exit(1);
    }

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error in creating socket");
        exit(2);
    }

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    // The inet_addr function converts a string containing an IPv4 
    // dotted-decimal address into a proper address for the IN_ADDR structure
    server_add.sin_addr.s_addr = inet_addr(argv[1]);
    server_add.sin_port = htons(atoi(argv[2]));

    printf("Trying to connect to server...\n");
    if(connect(sock_fd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0)
    {
        perror("Error in connecting to server");
        exit(3);
    }
    
    printf("Connected to server...\n");

    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recev_buffer, 0, sizeof(recev_buffer));
    while(fgets(send_buffer, LINE_LENGTH, stdin))
    {
        send(sock_fd, send_buffer, strlen(send_buffer), 0);

        if(recv(sock_fd, recev_buffer, LINE_LENGTH, 0) == 0)
        {
            perror("Server terminated prematurely");
            exit(4);
        }

        printf("Data received from server: %s", recev_buffer);
        memset(send_buffer, 0, sizeof(send_buffer));
        memset(recev_buffer, 0, sizeof(recev_buffer));
    }

    exit(0);
}