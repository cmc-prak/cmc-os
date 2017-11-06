#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

// перенаправление стандартных потоков
int main(int argc, char *argv[])
{
    int pid = fork();
    if (!pid) {
        int fd = open("wc.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);
        // нестандартный системный вызов dup3 позволяет указать
        // флаг принудительного закрытие при exec
        dup3(fd, 1, FD_CLOEXEC);
        close(fd);
        fd = open("fork7.c", O_RDONLY, 0);
        dup2(fd, 0);
        close(fd);
        execlp("./inter", "wc", "-c", NULL);
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
