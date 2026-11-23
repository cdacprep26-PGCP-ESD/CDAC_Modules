#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 10

struct data
{
    sem_t sem;   //Semaphore
    int temp[N]; // the 10 readings
};

int main()
{
    int id, i, pid;
    struct data *p;

    //Creating Shared Memery 
    id = shmget(IPC_PRIVATE, sizeof(struct data), IPC_CREAT | 0666);

    if (id < 0)
    {
        perror("shmget");
        exit(1);
    }

    //Memory Attachment
    p = shmat(id, NULL, 0);

    if (p == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    //Semaphore Initalliztion
    sem_init(&p->sem, 1, 0);

    printf("Parent (PID %d): shared memory created, id = %d\n", getpid(), id);

    printf("Parent Cretion Child\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        //Child Starts 
        int max, min, sum = 0;
        float avg;

        printf("Child  (PID %d): waiting for parent to finish writing\n", getpid());

        sem_wait(&p->sem);

        max = min = p->temp[0];

        for (i = 0; i < N; i++)
        {
            if (p->temp[i] > max)
            {
                max = p->temp[i];
            }

            if (p->temp[i] < min)
            {
                min = p->temp[i];
            }
            sum = sum + p->temp[i];
        }

        avg = (float)sum / N;

        printf("Child  (PID %d): max temperature = %d\n", getpid(), max);
        printf("Child  (PID %d): min temperature = %d\n", getpid(), min);
        printf("Child  (PID %d): avg temperature = %f\n", getpid(), avg);

        shmdt(p);
        exit(0);
    }

    //Parent sleeps 
    sleep(5); 

    srand(time(NULL));

    printf("Parent (PID %d): readings written:", getpid());

    for (i = 0; i < N; i++)
    {
        p->temp[i] = 20 + rand() % 26; //Generation Random Values
        printf(" %d", p->temp[i]);
    }

    printf("\n");

    sem_post(&p->sem); // writing done, child can read now

    printf("Parent (PID %d): writing done, semaphore posted\n", getpid());

    wait(NULL);
  

    //cleanup
    sem_destroy(&p->sem);
    shmdt(p);
    shmctl(id, IPC_RMID, NULL);
    printf("Parent (PID %d): semaphore destroyed, shared memory removed\n", getpid());

    return 0;
}