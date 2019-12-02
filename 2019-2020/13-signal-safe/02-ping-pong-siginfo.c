#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int fd[2];

void handler(int s, siginfo_t *pinfo, void *ptr)
{
    int pid = pinfo->si_pid;
    int x;
    read(fd[0], &x, sizeof(x));
    printf("%d %d %d\n", pid, getpid(), x); fflush(stdout);
    ++x;
    write(fd[1], &x, sizeof(x));
    if (pid > 0) {
        kill(pid, SIGUSR1);
    }
}

int main()
{
    pipe(fd);

    sigaction(SIGUSR1, &(struct sigaction) { .sa_sigaction = handler, .sa_flags = SA_SIGINFO }, NULL);

    int pid = fork();
    if (!pid) {
        //sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
        pid = getppid();

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
