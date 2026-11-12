#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_CHILDREN 3
#define FIFO_NAME "/tmp/os_lab_fifo"

int buffer;
sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg)
{
    int item = 100;

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer = item;
    printf("Producer: Produced item = %d\n", buffer);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return NULL;
}

void *consumer(void *arg)
{
    int item;

    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    item = buffer;
    printf("Consumer: Consumed item = %d\n", item);

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return NULL;
}

int main()
{
    pid_t pid[NUM_CHILDREN];
    int status;

    setbuf(stdout, NULL);

    printf("Parent Process Started\n");
    printf("Parent PID: %d\n\n", getpid());

    unlink(FIFO_NAME);

    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo failed");
        return 1;
    }

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid[i] = fork();

        if (pid[i] < 0)
        {
            perror("fork failed");
            return 1;
        }

        if (pid[i] == 0)
        {
            printf("Child %d created. PID = %d, PPID = %d\n", i + 1, getpid(), getppid());

            if (i == 0)
            {
                buffer = 0;
                sem_init(&empty, 0, 1);
                sem_init(&full, 0, 0);
                pthread_mutex_init(&mutex, NULL);
                printf("Child 1 : Create 2 threads\n");
                pthread_t t1, t2;

                pthread_create(&t1, NULL, producer, NULL);
                pthread_create(&t2, NULL, consumer, NULL);

                pthread_join(t1, NULL);
                pthread_join(t2, NULL);

                sem_destroy(&empty);
                sem_destroy(&full);
                pthread_mutex_destroy(&mutex);

                int fd = open(FIFO_NAME, O_RDONLY);

                if (fd == -1)
                {
                    perror("FIFO open failed");
                    exit(1);
                }

                char msg[100];

                int n = read(fd, msg, sizeof(msg) - 1);

                if (n < 0)
                {
                    perror("FIFO read failed");
                }
                else
                {
                    msg[n] = '\0';
                    printf("Child 1 received: %s\n", msg);
                }

                close(fd);
            }
            exit(0);
        }
        else
        {
            printf("Parent : Created child %d with PID = %d\n", i + 1, pid[i]);
        }
    }

    int fd = open(FIFO_NAME, O_WRONLY);

    if (fd == -1)
    {
        perror("FIFO open failed");
        return 1;
    }

    char msg[] = "Hello from Parent Process";

    write(fd, msg, strlen(msg) + 1);

    printf("Parent sent: %s\n", msg);

    close(fd);

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        waitpid(pid[i], &status, 0);

        printf("Parent: Child %d terminated\n", i + 1);
    }

    unlink(FIFO_NAME);

    return 0;
}