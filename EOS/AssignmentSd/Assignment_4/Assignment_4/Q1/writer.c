#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NPAGES 3
#define SHM_KEY 0x4545

int main(void)
{
    long psz = sysconf(_SC_PAGESIZE);
    size_t size = NPAGES * psz;

    int shmid = shmget(SHM_KEY, size, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    char *base = shmat(shmid, NULL, 0);
    if (base == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    printf("[Writer pid=%d] shmid=%d attached at %p\n", getpid(), shmid, (void *)base);

    for (int i = 0; i < NPAGES; i++)
    {
        char *page = base + i * psz;
        snprintf(page, psz, "Message from WRITER in page %d", i);
        printf("[Writer] wrote page %d at %p\n", i, (void *)page);
    }

    printf("\nRun ./reader in another terminal, then press Enter here...\n");
    getchar();

    printf("[Writer] page 2 now says: \"%s\"\n", base + 2 * psz);

    shmdt(base);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
