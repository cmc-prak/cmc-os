#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t usr_flag;

void handler(int s)
{
    usr_flag = 1;
}

void work(int pid, int fd[2])
{
    sigset_t s2;
    sigemptyset(&s2);

    while (1) {
        while (!usr_flag) {
            sigsuspend(&s2);
        }
        usr_flag = 0;
        int x;
        read(fd[0], &x, sizeof(x));
        printf("%d %d %d\n", pid, getpid(), x); fflush(stdout);
        ++x;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
}

int main()
{
    int fd[2];
    int pid;

    pipe(fd);

    sigset_t s1;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);

    sigprocmask(SIG_BLOCK, &s1, NULL);

    pid = fork();
    if (!pid) {
        sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
        work(getppid(), fd);


        _exit(0);
    }

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
    {
        int x = 1;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
    work(pid, fd);
}
