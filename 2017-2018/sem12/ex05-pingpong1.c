#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// классический пинг-понг между отцом и сыном с сигналами

int pid;
int fd[2];

void handler(int s)
{
    int x;
    if (!pid) pid = getppid();
    read(fd[0], &x, sizeof(x));
    printf("%d %d %d\n", getpid(), x, pid); fflush(stdout);
    ++x;
    write(fd[1], &x, sizeof(x));
    kill(pid, SIGUSR1);
}

int main()
{
    pipe(fd);
    signal(SIGUSR1, handler);
    pid = fork();
    if (!pid) {
        pid = getppid();
        while (1) { pause(); }
    } else {
        int z = 1;
        write(fd[1], &z, sizeof(z));
        kill(pid, SIGUSR1);
        while (1) { pause(); }
    }
}
