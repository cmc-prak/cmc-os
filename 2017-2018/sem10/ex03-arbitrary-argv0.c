#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int pid = fork();
    if (!pid) {
        // в качестве argv[0] в запускаемую программу может быть
        // передано что угодно
        execlp("./prog3.xx", "./fork3", "-l", NULL);
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
