#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHDMSIZE 512

int main(int argc, char** argv)
{
    int size = SHDMSIZE;
    //allocate memory
    int shm_id = shmget(12345, size, IPC_CREAT | 0666);

    //open memory
    char* shm_pointer = shmat(shm_id, 0, 0);

    int message = 3;

    printf("min: ");
    scanf("%d", &message);

    shm_pointer[0] = (char)message;

    //close memory
    shmdt(shm_pointer);


    printf("%d\n",shm_id);

    return 0;
}