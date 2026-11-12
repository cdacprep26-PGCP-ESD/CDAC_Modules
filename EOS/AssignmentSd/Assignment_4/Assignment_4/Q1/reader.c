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

    int shmid = shmget(SHM_KEY, size, 0666);
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
    printf("[Reader pid=%d] shmid=%d attached at %p\n", getpid(), shmid, (void *)base);

    for (int i = 0; i < NPAGES; i++)
    {
        char *page = base + i * psz;
        printf("[Reader] page %d at %p: \"%s\"\n", i, (void *)page, page);
    }

    snprintf(base + 2 * psz, psz, "Page 2 modified by READER");
    
    printf("\nReader waiting. Run ipcs in another terminal, then press Enter...\n");
    getchar();
    shmdt(base);
    return 0;
}
