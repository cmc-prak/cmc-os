#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void myexitfunc()
{
    fprintf(stderr, "atexit\n");
}

int main(int argc, char *argv[])
{
    atexit(myexitfunc);
    printf("Hello");
    int pid = fork();
    if (!pid) {
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "error: %s\n", strerror(errno));
        _exit(1); // после exec нужно завершать процесс по _exit()!
    }

    wait(NULL);
}
