#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048

int main()
{
    printf("usage:\ntype messege and hit enter\n type /spy to display the charcount on the server\n type close to shutdown the server and the client");

    char* fifo_name = "./wg_fifo";


    char buffer[BUFFER_SIZE];
    while(true)
    {
        //open
        int pipe_id = open(fifo_name, O_RDWR);
        if (pipe_id < 0)
        {
            printf("%d pipe is not open!\n", pipe_id);
            exit(-1);
        }

        printf("Enter message: ");
        scanf("%s", buffer);
        write(pipe_id, buffer, strlen(buffer) * sizeof(char));
        
        //close
        close(pipe_id);

        if (!strcmp(buffer, "close"))
            break;
    }
    exit(0);
}