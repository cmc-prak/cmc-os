#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// заголовочные файлы для SysV IPC семафоров
#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 0xdeadbeef };

/*
 * пример на создание/удаление и изменение свойств SysV IPC массивов семафоров
 */
int main(void)
{
    // получаем массив из 3 семафоров
    int semid = semget(KEY, 3, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }

    // устанавливаем все значения одновременно
    semctl(semid, 0, SETALL, (unsigned short[]) { 20, 0, 20000 });
    // устанавливаем значение семафора с индексом 1
    semctl(semid, 1, SETVAL, 10);
    // выводим значения семафоров 0 и 1
    printf("%d\n%d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));

    // удаляем массив семафоров
    semctl(semid, 0, IPC_RMID);
}
