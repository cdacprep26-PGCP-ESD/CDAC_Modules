#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define FIFO_NAME "/tmp/myfifo"
#define BUFFER_SIZE 5
#define MSG_SIZE 20
#define STOP -1

// Shared buffer
int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

// Synchronization variables
sem_t empty;
sem_t full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Read bytes from FIFO
int read_all(int fd, void *data, size_t size)
{
    char *ptr = (char *)data;
    size_t remaining = size;

    while (remaining > 0)
    {
        ssize_t n = read(fd, ptr, remaining);
        if (n == 0)
        {
            if (remaining == size)
                return 0;

            return -1;
        }

        if (n < 0)
        {
            if (errno == EINTR)
                continue;

            return -1;
        }
        ptr += n;
        remaining -= n;
    }

    return 1;
}

// Write exactly size bytes to FIFO
int write_all(int fd, void *data, size_t size)
{
    char *ptr = (char *)data;
    size_t remaining = size;

    while (remaining > 0)
    {
        ssize_t n = write(fd, ptr, remaining);

        if (n < 0)
        {
            if (errno == EINTR)
                continue;

            return -1;
        }

        ptr = ptr + n;
        remaining = remaining - n;
    }

    return 0;
}

// Producer thread
void *producer(void *arg)
{
    int fd;
    int item;
    int result;
    char msg[MSG_SIZE];

    printf("Producer thread started. TID");
    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("Producer FIFO open failed");
        return NULL;
    }
    if (read_all(fd, msg, MSG_SIZE) == 1)
    {
        printf("Producer received message from parent: %s\n", msg);
    }
    else
    {
        printf("Start message not received\n");
    }

    while (1)
    {
        result = read_all(fd, &item, sizeof(int));

        if (result == 0)
        {
            break;
        }

        if (result == -1)
        {
            perror("FIFO read failed");
            break;
        }

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    close(fd);

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[in] = STOP;
    in = (in + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    printf("Producer finished\n");

    return NULL;
}

// Consumer thread
void *consumer(void *arg)
{
    int item;

    printf("Consumer thread started. TID\n");

    while (1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        if (item == STOP)
        {
            break;
        }
        printf("Log ID %d Processed\n", item);
    }
    printf("Consumer finished\n");

    return NULL;
}

// Main function
int main()
{
    pthread_t tid1, tid2;
    pid_t pid;
    int status;
    int fd;
    int i;

    // Create FIFO
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo failed");
            exit(1);
        }
    }

    printf("FIFO created: %s\n", FIFO_NAME);
    fflush(stdout);

    // Create the child process
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0)
    {
        printf("\nChild created. PID = %d\n", getpid());
        printf("Child creating threads\n");

        //Initialize semaphores
        sem_init(&empty, 0, BUFFER_SIZE);
        sem_init(&full, 0, 0);

        //Create threads
        pthread_create(&tid1, NULL, producer, NULL);
        pthread_create(&tid2, NULL, consumer, NULL);

        //Wait for threads
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        //Destroy synchronization objects
        sem_destroy(&empty);
        sem_destroy(&full);
        pthread_mutex_destroy(&mutex);

        printf("Child: Both threads completed\n");
        exit(0);
    }

    // Parent sends data through FIFO
    printf("\nParent sending data\n");
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("Parent FIFO open failed");
        exit(1);
    }
    // start message
    char start_msg[MSG_SIZE] = "START";
    write_all(fd, start_msg, MSG_SIZE);
    printf("Parent sent start message: %s\n", start_msg);

    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (i = 0; i < 10; i++)
    {
        write_all(fd, &values[i], sizeof(int));
    }

    // Closing FIFO
    close(fd);

    // Collect child
    pid = waitpid(-1, &status, 0);
    if (pid > 0)
    {
        printf("\nCollected child PID = %d\n", pid);
        if (WIFEXITED(status))
        {
            printf("Exit status = %d\n", WEXITSTATUS(status));
        }
    }

    // Remove FIFO
    unlink(FIFO_NAME);

    printf("\nChild terminated\n");

    return 0;
}