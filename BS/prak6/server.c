#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>


int SERVER_PORT = 12121;
static int sock_len = sizeof(struct sockaddr_in);

void error_check(int err, const char* message);

void run_server(int socket_id, struct sockaddr* socket_config);

int count_lines(char* string, int len);

int main(int argc, char** argv)
{
    if (argc == 2)
        SERVER_PORT = atoi(argv[1]);

    struct sockaddr_in socket_configuration;
    //memset(&socket_configuration, 0, sizeof(socket_configuration));
    socket_configuration.sin_family = AF_INET;
    socket_configuration.sin_addr.s_addr = INADDR_ANY;
    socket_configuration.sin_port = htons(SERVER_PORT);


    int socket_id;
    error_check((socket_id = socket(AF_INET, SOCK_STREAM, 0)), "socket");

    error_check(bind(socket_id, (struct sockaddr*)&socket_configuration, sizeof(socket_configuration)), "bind");

    printf("Server Listening\n");
    
    run_server(socket_id, (struct sockaddr*)&socket_configuration);

    unlink((const char*)&socket_configuration);
    printf("Server Closed\n");
    exit(0);
}

void run_server(int socket_id, struct sockaddr* socket_config)
{
    //start listening, max 2 connections
    error_check(listen(socket_id, 2), "listen");

    int handle_socket_id;
    while (true)
    {
        printf("accepting\n");
        handle_socket_id = accept(socket_id, socket_config, &sock_len);

        const int MAXCHARS = 2048;
        char buffer[MAXCHARS];
        
        //RECIEVE
        printf("reading\n");
        size_t recv_len = recv(handle_socket_id, buffer, MAXCHARS, 0);

        buffer[recv_len] = '\0';
        printf("input:\n%s\n", buffer);

        //count
        int line_counter = count_lines(buffer, recv_len);

        //ANSWER
        const int MESSAGE_SIZE = 15;
        char send_message[MESSAGE_SIZE];
        sprintf(send_message, "%d Lines", line_counter);
        error_check(send(handle_socket_id, send_message, MESSAGE_SIZE, 0), "send");
        break;
    }
    close(handle_socket_id);
}

int count_lines(char* string, int len)
{
    int counter = 0;
    for (int i = 0; i < len; i++)
    {
        if (string[i] == '\n')
            counter++;
    }
    return counter;
}

void error_check(int err, const char* message)
{
    if (err == -1)
    {
        printf("Error: %s %d\n", message, errno);
        exit(-1);
    }
}