#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define NUM_ITEMS 10
#define MSG_DATA 1
#define MSG_END 2
struct int_msg
{
    long mtype;
    int value;
};

static int msqid;

static int slot;
static int slot_full = 0;
static int done = 0;
static int processed = 0;
static FILE *out;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t can_put = PTHREAD_COND_INITIALIZER;
static pthread_cond_t can_get = PTHREAD_COND_INITIALIZER;

static void *producer(void *arg)
{
    (void)arg;
    struct int_msg m;

    for (int i = 1; i <= NUM_ITEMS; i++)
    {
        m.mtype = MSG_DATA;
        m.value = 2 * i;

        if (msgsnd(msqid, &m, sizeof(int), 0) < 0)
        {
            perror("msgsnd");
            break;
        }
        printf("Producer: sent %d\n", m.value);
        usleep(100000);
    }

    m.mtype = MSG_END;
    m.value = 0;
    if (msgsnd(msqid, &m, sizeof(int), 0) < 0)
        perror("msgsnd END");
    printf("Producer: sent END message\n");

    return NULL;
}

static void *consumer(void *arg)
{
    (void)arg;
    struct int_msg m;

    for (;;)
    {
        if (msgrcv(msqid, &m, sizeof(int), 0, 0) < 0)
        {
            perror("msgrcv");
            break;
        }
        if (m.mtype == MSG_END)
            break;

        pthread_mutex_lock(&mtx);
        if (slot_full)
            printf("Consumer: buffer full, waiting...\n");
        while (slot_full)
            pthread_cond_wait(&can_put, &mtx);

        slot = m.value;
        slot_full = 1;
        printf("Consumer: received %2d from queue, placed in buffer\n", m.value);
        pthread_cond_signal(&can_get);
        pthread_mutex_unlock(&mtx);
    }

    pthread_mutex_lock(&mtx);
    done = 1;
    pthread_cond_signal(&can_get);
    pthread_mutex_unlock(&mtx);

    printf("Consumer: finished\n");
    return NULL;
}

static void *processor(void *arg)
{
    (void)arg;

    for (;;)
    {
        pthread_mutex_lock(&mtx);
        if (!slot_full && !done)
            printf("Processor: buffer empty, waiting...\n");
        while (!slot_full && !done)
            pthread_cond_wait(&can_get, &mtx);

        if (!slot_full && done)
        {
            pthread_mutex_unlock(&mtx);
            break;
        }

        int n = slot;
        slot_full = 0;
        pthread_cond_signal(&can_put);
        pthread_mutex_unlock(&mtx);

        fprintf(out, "Number %d -> square = %d\n", n, n * n);
        processed++;
        printf("Processor: took %2d, square = %3d, written to output.txt\n", n, n * n);
        usleep(300000);
    }

    printf("Processor: finished\n");
    return NULL;
}

static void producer_child(void)
{
    pthread_t t;

    printf("Child 1 (pid %d) creating the producer thread\n", getpid());
    if (pthread_create(&t, NULL, producer, NULL) != 0)
    {
        fprintf(stderr, "Child 1: pthread_create failed\n");
        exit(1);
    }
    pthread_join(t, NULL);
    printf("Child 1 finished\n");
    exit(0);
}

static void consumer_child(void)
{
    pthread_t tc, tp;

    printf("Child 2 (pid %d) creating the consumer and processing threads\n", getpid());

    out = fopen("output.txt", "w");
    if (out == NULL)
    {
        perror("fopen output.txt");
        exit(1);
    }

    if (pthread_create(&tc, NULL, consumer, NULL) != 0 ||
        pthread_create(&tp, NULL, processor, NULL) != 0)
    {
        fprintf(stderr, "Child 2: pthread_create failed\n");
        exit(1);
    }

    pthread_join(tc, NULL);
    pthread_join(tp, NULL);

    fclose(out);
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&can_put);
    pthread_cond_destroy(&can_get);

    printf("Child 2 finished: %d numbers processed\n", processed);
    exit(processed == NUM_ITEMS ? 0 : 1);
}

int main(void)
{
    pid_t pid[2];
    const char *names[2] = {"Child 1 (producer)", "Child 2 (consumer)"};

    setbuf(stdout, NULL);

    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msqid < 0)
    {
        perror("msgget");
        return 1;
    }
    printf("Parent (pid %d): message queue created, id %d\n", getpid(), msqid);

    for (int i = 0; i < 2; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("fork");
            msgctl(msqid, IPC_RMID, NULL);
            return 1;
        }

        if (pid[i] == 0)
        {
            if (i == 0)
                producer_child();
            else
                consumer_child();
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

    msgctl(msqid, IPC_RMID, NULL);
    printf("Parent: message queue removed\n");

    return 0;
}