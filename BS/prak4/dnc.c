#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void print_dir(char path[]);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("usage ./dnc dir\n");
        exit(-1);
    }

    printf("main:\nprocess: %d\n", getpid());
    print_dir(argv[1]);


    printf("\nwait for children: %d\n", getpid());



    /*
    waitpid(-1, NULL, 0);
    
       < -1   meaning wait for any child process whose process group ID is
              equal to the absolute value of pid.

       -1     meaning wait for any child process.

       0      meaning wait for any child process whose process group ID is
              equal to that of the calling process.
              
       > 0    meaning wait for the child whose process ID is equal to the
              value of pid.
    */
    waitpid(-1, NULL, 0);
    printf("\nexit process: %d\n", getpid());
    exit(0);
}


void print_dir(char path[])
{
    char slash[2] = { '/', '\0'};
    DIR* dp = opendir(path);

    chdir(path);

    struct dirent *ep;
    int status;
    struct stat st_buf;

    while (ep = readdir (dp))
    {
        if (!(ep->d_name[0] == '.'))
        {
            printf("\ndir: %s\n", path);
            printf("elem: %s", ep->d_name);
        }
        status = stat (ep->d_name, &st_buf);

        if (S_ISDIR(st_buf.st_mode) && !((strcmp(ep->d_name, ".") == 0) || (strcmp(ep->d_name, "..") == 0) || (ep->d_name[0] == '.')))
        {
            char new_path[256] = { '.' };
            strcat(new_path, slash);
            strcat(new_path, ep->d_name);
            
            printf("\nnew fork\ninitiating process: %d", getpid());
            pid_t pid = fork();
            if (pid == 0)
            {
                printf("\n\nnew dir\nprocess: %d", getpid());
                print_dir(new_path);
                printf("\nexit process: %d\n", getpid());
                exit(0);
            }
        }
    }
    (void) closedir (dp);
}