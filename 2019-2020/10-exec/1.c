#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
    } else if (!pid) {
        if (chdir(argv[1]) < 0) {
            fprintf(stderr, "cannot chdir to '%s'\n", argv[1]);
            _exit(1);
        }
        char *myenv[] =
        {
            "A=1",
            "B=2",
            NULL
        };
        execve(argv[2], &argv[2], myenv);
        fprintf(stderr, "exec of '%s' failed\n", argv[2]);
        _exit(1);
    }

    wait(NULL);
}
