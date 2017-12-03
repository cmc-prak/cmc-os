#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 0xdeadbeef };
enum { COUNT = 2 };         // число пингпонгующихся процессов
enum { INIT_VAL = 132 };    // начальное значение, которым обмениваемся

void handler(int s)
{
}

/*
 * пример на "пинг-понг" без ограничения на максимальное значение
 * программа "зависнет" при достижении значения 32767
 * семафор [0] хранит пересылаемое значение
 *         [1] открывается, когда приходит очередь первого сына
 *         [2] открывается, когда приходит очередь второго сына
 */
int main(void)
{
    int semid = semget(KEY, 3, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    // устанавливаем начальное значение
    semctl(semid, 0, SETALL, (unsigned short[]) { INIT_VAL, 1, 0 });

    for (int i = 1; i <= COUNT; ++i) {
        if (!fork()) {
            while (1) {
                // ждем своей очереди
                struct sembuf ops[1] =
                {
                    { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
                };
                if (semop(semid, ops, 1) < 0) {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(1);
                }
                printf("%d %d\n", i, semctl(semid, 0, GETVAL)); fflush(stdout);
                // передаем ход следующему и увеличиваем передаваемое значение
                semop(semid, (struct sembuf[]) {{ .sem_num = 3 - i, .sem_op = 1, .sem_flg = 0}, {.sem_num = 0, .sem_op = 1, .sem_flg = 0}}, 2);
            }
        }
    }

    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    int r;
    while ((r = wait(NULL)) > 0) {}
    fprintf(stderr, "wait: %s\n", strerror(errno));

    semctl(semid, 0, IPC_RMID);
}
