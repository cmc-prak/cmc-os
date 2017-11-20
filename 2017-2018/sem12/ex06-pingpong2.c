#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// пинг-понг на сигналах здорового человека
// нет, здоровый человек не использует сигналы!

volatile int flag = 0;

void handler()
{
    flag = 1;
}

int main()
{
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler }, NULL);
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (!pid) {
        pid = getppid();
    } else {
        int z = 1;
        write(fd[1], &z, sizeof(z));
        kill(pid, SIGUSR1);
    }

    while (1) {
        /*
        // этот фрагмент кода содержит race condition
        // сигнал может придти в момент после первого sigprocmask
        // но до pause, тогда pause() заснет на неопределенное время
        // sigsuspend работает атомарно
        while (!flag) {
            sigprocmask(SIG_SETMASK, &s2, NULL);
            pause();
            sigprocmask(SIG_BLOCK, &s1, NULL);
        }
        */
        while (!flag) {
            // атомарная замена последовательности sigprocmask / pause / sigprocmask
            sigsuspend(&s2);
        }

        flag = 0;
        int x;
        read(fd[0], &x, sizeof(x));
        printf("%d %d\n", getpid(), x);
        ++x;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
}
