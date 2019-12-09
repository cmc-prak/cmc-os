#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>

void work(int semid, int64_t *ptr, int s1, int s2)
{
    while (1) {
        struct sembuf b1[] =
        {
            { .sem_num = s1, -1, 0 },
        };
        int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
        if (r < 0) {
            fprintf(stderr, "semop: %s\n", strerror(errno));
            exit(1);
        }

        printf("%d %lld\n", getpid(), (long long) *ptr); fflush(stdout);
        ++(*ptr);

        struct sembuf b2[] =
        {
            { .sem_num = s2, 1, 0 },
        };
        r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
        if (r < 0) {
            fprintf(stderr, "semop: %s\n", strerror(errno));
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    key_t key = strtol(argv[1], NULL, 16);

    int semid = semget(key, 3, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        exit(1);
    }

    semctl(semid, 1, SETVAL, 1);

    int64_t *ptr = mmap(NULL, sizeof(ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    if (!fork()) {
        work(semid, ptr, 1, 2);
        _exit(0);
    }

    if (!fork()) {
        work(semid, ptr, 2, 1);
        _exit(0);
    }

    wait(NULL);
    wait(NULL);
}
