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
#include <signal.h>

volatile sig_atomic_t int_flag = 0;
void handler(int s)
{
    int_flag = 1;
}

void work(int semid, int index, char *status, int count)
{
    while (1) {
        int f1 = index;
        int f2 = (index + 1) % count;

        status[index] = 'W';
        struct sembuf b1[] =
        {
            { .sem_num = f1, -1, SEM_UNDO },
            { .sem_num = f2, -1, SEM_UNDO },
        };
        int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
        if (r < 0) {
            if (errno == EIDRM) {
                fprintf(stderr, "exiting\n"); 
            }
            break;
        }
        status[index] = 'E';
        usleep(100000);
        struct sembuf b2[] =
        {
            { .sem_num = f1, 1, SEM_UNDO },
            { .sem_num = f2, 1, SEM_UNDO },
        };
        r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
        if (r < 0) {
            if (errno == EIDRM) {
                fprintf(stderr, "exiting\n"); 
            }
            break;
        }
        status[index] = 'S';
        usleep(100000);
    }
}

int main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, 10);
    key_t key = strtol(argv[2], NULL, 16);

    char *status = mmap(NULL, count + 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    memset(status, '-', count);
    status[count] = 0;

    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        exit(1);
    }

    short *ptr = calloc(count, sizeof(ptr[0]));
    for (int i = 0; i < count; ++i) ptr[i] = 1;
    semctl(semid, 0, SETALL, ptr);

    for (int i = 0; i < count; ++i) {
        if (!fork()) {
            work(semid, i, status, count);
            _exit(0);
        }
    }

    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler }, NULL);

    while (!int_flag) {
        printf("%s\r", status); fflush(stdout);
        usleep(100000);
    }

    semctl(semid, 0, IPC_RMID);

    while (wait(NULL) > 0) {}
}
