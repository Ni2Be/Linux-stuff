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

void run_client(int socket_id, struct sockaddr* socket_config);


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

    error_check(connect(socket_id, (struct sockaddr*)&socket_configuration, sizeof(socket_configuration)), "connect");

    printf("Client Started\n");
    
    run_client(socket_id, (struct sockaddr*)&socket_configuration);

    unlink((const char*)&socket_configuration);
    printf("Client Closed\n");
    exit(0);
}

void run_client(int socket_id, struct sockaddr* socket_config)
{
    while (true)
    {
        printf("sending\n");

        const int MAXCHARS = 2048;
        char message[MAXCHARS];
        
        //GEN MESSAGE
        message[0] = '\0';
        strcat( message, "Lorem ipsum dolor sit amet, \n");
        strcat( message, "consectetur adipiscing elit. Pellentesque nunc lorem,\n");
        strcat( message, "laoreet id est tristique, consectetur lacinia arcu. \n");
        strcat( message, "Curabitur eget ornare arcu. \n");
        strcat( message, "Mauris lectus sem, blandit ut ullamcorper et.\n");
        strcat( message, "END\n");
        
        //SEND
        send(socket_id, message, strlen(message), 0);

        //RECIEVE
        char answer[MAXCHARS];
        printf("reading\n");
        size_t recv_len = recv(socket_id, answer, MAXCHARS, 0);
        answer[recv_len] = '\0';
        printf("answer: \"%s\"\n", answer);

        
        break;
    }
    close(socket_id);
}


void error_check(int err, const char* message)
{
    if (err == -1)
    {
        printf("Error: %s %d\n", message, errno);
        exit(-1);
    }
}