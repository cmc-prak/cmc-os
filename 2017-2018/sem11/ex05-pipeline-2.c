#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// использование Linux-specific pipe2 и флага
// принудительного закрытия файловых дескрипторов при exec
int main(int argc, char *argv[])
{
    int fds[2];

    pipe2(fds, O_CLOEXEC);
    if (!fork()) {
        dup2(fds[1], 1);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    if (!fork()) {
        dup2(fds[0], 0);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    close(fds[1]);
    close(fds[0]);
    wait(NULL);
    wait(NULL);
}
