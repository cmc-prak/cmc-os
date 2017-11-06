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
        raise(SIGSEGV);
        //_exit(500);
    }
    int pid2 = fork();
    if (!pid2) {
        _exit(120);
    }

    //usleep(100);
    int status;
    // waitpid позволяет проверить, завершился ли какой-либо
    // из процессов-сыновей без приостановки выполнения
    // процесса-родителя
    int p = waitpid(-1, &status, WNOHANG);
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
