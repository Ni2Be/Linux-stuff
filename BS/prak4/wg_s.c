#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048

int main()
{
    printf("usage: the server is waiting for input throught the clients\ntype close in one of the clients to shutdown the server and the client");
    char* fifo_name = "./wg_fifo";

    //create
    printf("create pipe\n");
    if (mkfifo(fifo_name, 0666) != 0)
    {
        if (errno != EEXIST) 
        {
            printf("could not create pipe\n");
            exit(-1);
        }
    }
        
    //open
    printf("open pipe\n");
    int pipe_id;
    pipe_id = open(fifo_name, O_RDWR);
    if (pipe_id == -1)
        printf("could not open pipe %d\n", pipe_id);

    char buffer[BUFFER_SIZE];
    int char_counter = 0;
    while(true)
    {
        printf("listening\n");
        int read_size = read(pipe_id, buffer, BUFFER_SIZE);
        char_counter += (read_size / sizeof(char));
        buffer[read_size] = '\0';
        if (read_size > 0)
            printf("%d byte: %s\n", read_size, buffer);
        if (!strcmp(buffer, "close"))
            break;
        if (!strcmp(buffer, "/spy"))
            printf("read %d chars\n", char_counter);;
    }
    close(pipe_id);
    unlink(fifo_name);

    exit(0);
}