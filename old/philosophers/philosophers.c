#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>
#include <signal.h>

int semid, shmid;

void inthnd(int s)
{
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

int main(int argc, char *argv[])
{
    key_t semk = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    int seed = strtol(argv[3], NULL, 10);

    signal(SIGINT, inthnd);

    semid = semget(semk, n, 0600 | IPC_CREAT);
    if (semid < 0) {
        perror("");
        exit(1);
    }

    shmid = shmget(semk, n + 1, 0600 | IPC_CREAT);
    char *mem = shmat(shmid, NULL, 0);
    
    short initval[n];
    //memset(initval, 0, sizeof(initval));
    for (int i = 0; i < n; ++i) {
        initval[i] = 1;
        mem[i] = '?';
    }
    semctl(semid, 0, SETALL, initval);
    mem[n] = 0;


    for (int i = 0; i < n; ++i) {
        if (!fork()) {
            srand(seed + i);
            while (1) {
                // waiting
                mem[i] = 'S';
                int wt = (int) (rand() / (RAND_MAX + 1.0) * 10) + 1;
                usleep(100000 * wt);

                int f1 = i;
                int f2 = (i + 1) % n;
                /*
                if (f1 > f2) {
                    int t = f1; f1 = f2; f2 = t;
                }
                */

                mem[i] = 'W';
                semop(semid, (struct sembuf[2]) {{ .sem_num = f1, .sem_op = -1, .sem_flg = SEM_UNDO }, { .sem_num = f2, .sem_op = -1, .sem_flg = SEM_UNDO }}, 2);

                // eating
                mem[i] = 'E';
                wt = wt / 2 + 1;
                usleep(100000 * wt);

                mem[i] = 'R';
                //usleep(100000);
                semop(semid, (struct sembuf[1]) {{ .sem_num = f2, .sem_op = 1, .sem_flg = SEM_UNDO }}, 1);
                semop(semid, (struct sembuf[1]) {{ .sem_num = f1, .sem_op = 1, .sem_flg = SEM_UNDO }}, 1);
            }
            exit(0);
        }
    }

    while (1) {
        usleep(100000);
        printf("State: %s\r", mem);fflush(stdout);
    }
}
