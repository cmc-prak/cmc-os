#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/signalfd.h>

void work(int sfd, int fd[2])
{
    sigset_t s2;
    sigemptyset(&s2);

    while (1) {
        int x;
        struct signalfd_siginfo ssi;
        read(sfd, &ssi, sizeof(ssi));
        read(fd[0], &x, sizeof(x));
        printf("%d %d %d\n", ssi.ssi_pid, getpid(), x); fflush(stdout);
        ++x;
        write(fd[1], &x, sizeof(x));
        kill(ssi.ssi_pid, SIGUSR1);
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

    int sfd = signalfd(-1, &s1, 0);

    pid = fork();
    if (!pid) {
        work(sfd, fd);


        _exit(0);
    }

    {
        int x = 1;
        write(fd[1], &x, sizeof(x));
        kill(pid, SIGUSR1);
    }
    work(sfd, fd);
}
