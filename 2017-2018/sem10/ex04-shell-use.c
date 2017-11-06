#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// использование стандартного интерпретатора
// командной строки /bin/sh
int main(int argc, char *argv[])
{
    int pid = fork();
    if (!pid) {
        // process argv[1]
        execlp("/bin/sh", "sh", "-c", argv[1], NULL);
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
