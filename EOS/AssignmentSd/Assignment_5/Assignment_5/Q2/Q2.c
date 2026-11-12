#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BUF_SIZE   5      
#define NUM_ITEMS  10    

typedef struct {
    int produced;
    int consumed;
} shared_status;

typedef struct {
    int buffer[BUF_SIZE];          
    int in;                        
    int out;                      
    sem_t empty;                  
    sem_t full;                   
    pthread_rwlock_t status_lock;  
    shared_status status;          
} shared_data;

static shared_data *sh;            

static void get_status(int *produced, int *consumed)
{
    pthread_rwlock_rdlock(&sh->status_lock);
    *produced = sh->status.produced;
    *consumed = sh->status.consumed;
    pthread_rwlock_unlock(&sh->status_lock);
}

static void *producer(void *arg)
{
    (void)arg;

    for (int item = 1; item <= NUM_ITEMS; item++)
    {
        sem_wait(&sh->empty);                    
        sh->buffer[sh->in] = item;
        sh->in = (sh->in + 1) % BUF_SIZE;
        sem_post(&sh->full);                    

        pthread_rwlock_wrlock(&sh->status_lock);
        sh->status.produced++;
        pthread_rwlock_unlock(&sh->status_lock);

        int p, c;
        get_status(&p, &c);
        printf("Producer: produced %2d   [status: produced=%d consumed=%d]\n", item, p, c);
    }
    return NULL;
}

static void *consumer(void *arg)
{
    (void)arg;

    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL)
    {
        perror("fopen output.txt");
        return NULL;
    }

    for (int n = 0; n < NUM_ITEMS; n++)
    {
        sem_wait(&sh->full);                   
        int item = sh->buffer[sh->out];
        sh->out = (sh->out + 1) % BUF_SIZE;
        sem_post(&sh->empty);                 

        int result = item * item;               
        fprintf(fp, "Item %d -> square = %d\n", item, result);

        pthread_rwlock_wrlock(&sh->status_lock);
        sh->status.consumed++;
        pthread_rwlock_unlock(&sh->status_lock);

        int p, c;
        get_status(&p, &c);
        printf("Consumer: consumed %2d -> %3d   [status: produced=%d consumed=%d]\n",
               item, result, p, c);

        usleep(300000);                          
    }

    fclose(fp);
    return NULL;
}

static void run_child(void *(*worker)(void *), const char *name)
{
    pthread_t t;

    printf("%s (pid %d) creating its worker thread\n", name, getpid());

    if (pthread_create(&t, NULL, worker, NULL) != 0)
    {
        fprintf(stderr, "%s: pthread_create failed\n", name);
        exit(1);
    }

    pthread_join(t, NULL);
    shmdt(sh);                               
    printf("%s finished\n", name);
    exit(0);
}

int main(void)
{
    pid_t pid[2];
    const char *names[2] = { "Child 1 (producer)", "Child 2 (consumer)" };

    setbuf(stdout, NULL);

    int shmid = shmget(IPC_PRIVATE, sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        return 1;
    }

    sh = shmat(shmid, NULL, 0);
    if (sh == (void *)-1)
    {
        perror("shmat");
        return 1;
    }
    printf("Parent (pid %d): shared memory id %d, size %zu bytes\n",
           getpid(), shmid, sizeof(shared_data));

    memset(sh, 0, sizeof(*sh));
    sem_init(&sh->empty, 1, BUF_SIZE);         
    sem_init(&sh->full, 1, 0);

    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_rwlock_init(&sh->status_lock, &attr);
    pthread_rwlockattr_destroy(&attr);

    for (int i = 0; i < 2; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("fork");
            shmctl(shmid, IPC_RMID, NULL);
            return 1;
        }

        if (pid[i] == 0)
        {
            if (i == 0)
                run_child(producer, names[0]);
            else
                run_child(consumer, names[1]);
        }
        printf("Parent: created %s, pid %d\n", names[i], pid[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        int status;
        waitpid(pid[i], &status, 0);

        if (WIFEXITED(status))
            printf("Parent: %s terminated, exit status %d\n", names[i], WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Parent: %s killed by signal %d\n", names[i], WTERMSIG(status));
    }

    int p, c;
    get_status(&p, &c);
    printf("Parent: final status: produced=%d consumed=%d\n", p, c);
    if (p == NUM_ITEMS && c == NUM_ITEMS)
        printf("Parent: all %d integers were produced, consumed and processed\n", NUM_ITEMS);

    sem_destroy(&sh->empty);
    sem_destroy(&sh->full);
    pthread_rwlock_destroy(&sh->status_lock);
    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Parent: synchronisation objects destroyed, shared memory removed\n");

    return 0;
}