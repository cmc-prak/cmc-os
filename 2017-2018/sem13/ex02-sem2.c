#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 0xdeadbeef };
enum { COUNT = 10 };
enum { INIT = 6 };

// пустой обработчик сигнала SIGINT
void handler(int s)
{
}

int main()
{
    // создаем массив из одного семафора
    int semid = semget(KEY, 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    // устанавливаем начальное значение семафора
    semctl(semid, 0, SETVAL, INIT);

    for (int i = 1; i <= COUNT; ++i) {
        if (!fork()) {
            while (1) {
                // отдельно объявляем массив операций с семафором
                struct sembuf ops[1] =
                {
                    { .sem_num = 0, .sem_op = -i, .sem_flg = 0 },
                };
                // операция down(i) над семафором 0
                if (semop(semid, ops, 1) < 0) {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(1);
                }
                printf("%d in\n", i); fflush(stdout);
                usleep(100000);
                printf("%d out\n", i); fflush(stdout);
                // массив операций объявлен как параметр в вызове semop
                // операция up(i) над семафором 0
                semop(semid, (struct sembuf[]) {{ .sem_num = 0, .sem_op = i, .sem_flg = 0}}, 1);
            }
        }
    }

    // в отце устанавливаем обработчик SIGINT с семантикой перезапускаемых системных вызовов
    // обработчик SIGINT нужен, чтобы отец не завершился из-за сигнала SIGINT
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    // когда созданные отцом процессы получат SIGINT, они будут завершаться
    // системный вызов wait() будет возвращать PID этих процессов,
    // пока не останется ни одного процесса, после чего wait() вернет -1, и цикл закончится
    int r;
    while ((r = wait(NULL)) > 0) {}
    fprintf(stderr, "wait: %s\n", strerror(errno));

    // удаляем массив семафоров
    semctl(semid, 0, IPC_RMID);
}
