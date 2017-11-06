#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// получение статуса завершения процесса с помощью wait
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

    int status;
    int p = wait(&status);
    printf("%d\n", p);
    if (WIFEXITED(status)) {
        printf("%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("%d\n", WTERMSIG(status));
    }
}
