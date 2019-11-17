#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd[2];

    pipe2(fd, O_CLOEXEC);
    pid_t pid1 = fork();
    if (!pid1) {
        dup2(fd[1], 1);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    pid_t pid2 = fork();
    if (!pid2) {
        dup2(fd[0], 0);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    close(fd[1]);
    close(fd[0]);

    while (wait(NULL) > 0) {}
}
