
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHDMSIZE 512

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("needs shm_id");
        exit(0);
    }

    int shm_id = atoi(argv[1]);

    //open memory
    char* shm_pointer =  shmat(shm_id, NULL, 0);

    int message = (int)*shm_pointer;

    printf("min: %d\n",message);

    if(message < 14*13/60)
        printf("got it\n");
    else
        printf("nope\n");

    //colse memory
    shmdt(shm_pointer);

    //free memory
    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}