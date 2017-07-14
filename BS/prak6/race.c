#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define PROCESCOUNT 50
#define PROCESRUNS 10000
#define SHDMSIZE 512

int mutex_id;
int mutex = 1;
struct sembuf sem_up = {0,1,SEM_UNDO};
struct sembuf sem_down = {0,-1,SEM_UNDO};


int start_the_fun(int* shm_ptr);
void child_labor(int add_val, int* valu_ptr);

int main()
{   
    //get mutex
    mutex_id = semget( 123456, 1, IPC_CREAT | 0666 );
    //bind memory to mutex
    semctl(mutex_id, 1, SETALL, &mutex);

    //allocate memory
    int shm_id = shmget(12345, SHDMSIZE, IPC_CREAT | 0666);
    //open memory
    int* shm_pointer = shmat(shm_id, 0, 0);
    shm_pointer[0] = 0;

    //start it
    pid_t pid = start_the_fun(shm_pointer);
    
    printf("parent %d waits\n", pid);
    int n = PROCESCOUNT;
    while(n > 0)
    {
        int status;
        waitpid(-1, NULL, 0);
        n--;
    }
    printf("value = %d\n", shm_pointer[0]);

    //colse memory
    shmdt(shm_pointer);
    //free memory
    shmctl(shm_id, IPC_RMID, 0);

    //free mutex
    semctl(mutex_id, 1, IPC_RMID);

    printf("parent %d finished\n", pid);
    exit(0);
}


int start_the_fun(int* shm_ptr)
{
    pid_t pid;
    for(int i = 0; i < PROCESCOUNT; i++)
    {
        pid = fork();
        printf("child %d produced\n", i);
        if (pid == 0 && i % 2 == 0)
        {
            child_labor(34, shm_ptr);
            printf("child %d exit\n", i);
            exit(0);
        }
        else if (pid == 0 && i % 2 == 1)
        {
            child_labor(-34, shm_ptr);
            printf("child %d exit\n", i);
            exit(0);
        }
    }
    return pid;
}


void child_labor(int add_val, int* valu_ptr)
{
    time_t t;
    srand(time(&t));

    usleep(rand() % 200);
    for (int n = 0; n < PROCESRUNS; n++)
    {
        //lock
        semop(mutex_id, &sem_down, 1);
        if (mutex > 0)
        {
            valu_ptr[0] += add_val;
        }
        //release
        semop(mutex_id, &sem_up, 1);
        
        //printf("%d\n",valu_ptr[0]);
        usleep(rand() % 2);
    }
}