#include <stdio.h>
#include <unistd.h>

#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(int argc, char** args)
{
    if (argc != 2)
    {
        printf("usage: ./ww dir");
    }

    char* path = args[1];
    DIR* dp = opendir(path);

    struct dirent *ep;
    int status;
    struct stat st_buf;

    int pipe_field[2];
    pipe(pipe_field);

    int pid = fork();
    //is parent
    if (pid != 0)
    {
        close(pipe_field[READ]);
        
        while (ep = readdir (dp))
        {
            write(pipe_field[WRITE], ep->d_name, strlen(ep->d_name) + 1);
        }
        close(pipe_field[WRITE]);
    }
    else 
    //is child
    {
        close(pipe_field[WRITE]);

        char buffer[100];
        
        int count = read(pipe_field[READ], buffer, 100);
        for (int i = 0; i < count; i++)
        {
            char word[100];
            int n = 0;
            while(buffer[i] != '\0')
            {
                word[n++] = buffer[i++];
            }
            word[n] = '\0';

            if (word[0] == '.' && word[1] != '.' && word[1] != '\0')
                printf("%s\n", word);
        }
        close(pipe_field[READ]);
    }

    waitpid(-1, NULL, 0);
    printf("\nexit process: %d\n", getpid());
    exit(0);
}