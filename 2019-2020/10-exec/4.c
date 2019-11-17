#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int sys(const char *cmd)
{
    pid_t pid = fork();
    if (pid < 0) {
    } else if (!pid) {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        _exit(1);
    }

    wait(0);
}

int main(int argc, char *argv[])
{
    sys(argv[1]);
    sys(argv[2]);
}
