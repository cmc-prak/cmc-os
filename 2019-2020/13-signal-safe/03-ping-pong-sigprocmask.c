#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int fd[2];
int pid;

void handler(int s)
{
    int x;
    read(fd[0], &x, sizeof(x));
    printf("%d %d %d\n", pid, getpid(), x); fflush(stdout);
    ++x;
    write(fd[1], &x, sizeof(x));
    kill(pid, SIGUSR1);
}

int main()
{
    pipe(fd);

    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigemptyset(&s2);

    sigprocmask(SIG_BLOCK, &s1, NULL);

    pid = fork();
    if (!pid) {
        sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
        pid = getppid();

        sigprocmask(SIG_UNBLOCK, &s1, NULL);
        while (1) {
            pause();
        }

        _exit(0);
    }

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
    {
        int x = 1;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
    sigprocmask(SIG_UNBLOCK, &s1, NULL);

    while (1) {
        pause();
    }
}
