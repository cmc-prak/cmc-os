#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 0xdeadbeef };
enum { COUNT = 4 };

/*
 * пример на "инвертированную" семантику семафоров:
 *   0 - семафор открыт
 *   1 - семафор закрыт
 */
int main(void)
{
    int semid = semget(KEY, 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }

    for (int i = 1; i <= COUNT; ++i) {
        if (!fork()) {
            while (1) {
                // ждем значения 0 на семафоре и сразу же устанавливаем его в 1
                // обе операции будут выполняться атомарно в совокупности
                // для "инвертированной" семантики это - закрытие семафора
                struct sembuf ops[2] =
                {
                    { .sem_num = 0, .sem_op = 0, .sem_flg = 0 },
                    { .sem_num = 0, .sem_op = 1, .sem_flg = 0 },
                };
                if (semop(semid, ops, 2) < 0) {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(1);
                }
                printf("%d in\n", i); fflush(stdout);
                usleep(100000);
                printf("%d out\n", i); fflush(stdout);
                // вычитаем 1 из значения семафора
                // для "инвертированной" семантики это - открытие семафора
                semop(semid, (struct sembuf[]) {{ .sem_num = 0, .sem_op = -1, .sem_flg = 0}}, 1);
            }
        }
    }

    // игнорирование SIGINT в отце будет иметь такой же эффект, как и
    // установка пустого обработчика (см. предыдущий пример)
    signal(SIGINT, SIG_IGN);

    int r;
    while ((r = wait(NULL)) > 0) {}
    fprintf(stderr, "wait: %s\n", strerror(errno));

    semctl(semid, 0, IPC_RMID);
}
