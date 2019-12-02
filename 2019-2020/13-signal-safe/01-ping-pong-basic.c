#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int fd[2];
int pid;

void handler(int s)
{
    int x;
    if (!pid) pid = getppid();
    read(fd[0], &x, sizeof(x));
    printf("%d %d %d\n", pid, getpid(), x); fflush(stdout);
    ++x;
    write(fd[1], &x, sizeof(x));
    kill(pid, SIGUSR1);
}

int main()
{
    pipe(fd);

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);

    pid = fork();
    if (!pid) {

        while (1) {
            pause();
        }

        _exit(0);
    }

    {
        int x = 1;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }

    while (1) {
        pause();
    }
}
