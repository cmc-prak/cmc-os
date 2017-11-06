#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

// передача произвольного окружения
// в программу, запускаемую с помощью системного вызова execve
int main(int argc, char *argv[])
{
    int pid = fork();
    if (!pid) {
        char *args[2] = { "prog.py", NULL };
        char *env[2] = { "var=value", NULL };
        execve("/bin/sh", args, NULL);
        fprintf(stderr, "exec failed: %s\n", strerror(errno));
        _exit(1);
    }

    int status;
    int p = waitpid(-1, &status, 0);
    if (p < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    if (!p) {
        printf("no processes\n");
        exit(1);
    }
    printf("%d\n", p);
    if (WIFEXITED(status)) {
        printf("%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("%d\n", WTERMSIG(status));
    }
}
