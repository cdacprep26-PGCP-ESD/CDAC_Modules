#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NPAGES 3

int main(void)
{
    long psz = sysconf(_SC_PAGESIZE);
    size_t size = NPAGES * psz;
    printf("Page size = %ld bytes, segment size = %zu bytes\n", psz, size);

    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    printf("shmid = %d\n", shmid);

    char *base = shmat(shmid, NULL, 0);
    if (base == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    printf("Parent attached at %p\n", (void *)base);

    for (int i = 0; i < NPAGES; i++)
    {
        char *page = base + i * psz;
        snprintf(page, psz, "Hello from PARENT, page %d", i);
        printf("Parent wrote page %d at %p: \"%s\"\n", i, (void *)page, page);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        printf("\n[Child pid=%d] attached at %p\n", getpid(), (void *)base);
        for (int i = 0; i < NPAGES; i++)
        {
            char *page = base + i * psz;
            printf("[Child] page %d at %p: \"%s\"\n", i, (void *)page, page);
        }
        snprintf(base, psz, "Page 0 modified by CHILD");
        shmdt(base);
        exit(0);
    }

    waitpid(pid, NULL, 0);
    printf("\n[Parent] page 0 after child finished: \"%s\"\n", base);

    shmdt(base);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
