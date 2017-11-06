#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <asm/unistd_32.h>

void myexitfunc()
{
    fprintf(stderr, "atexit\n");
}

int main(int argc, char *argv[])
{
    atexit(myexitfunc);
    printf("Hello");
    int pid;
    // результат системного вызова fork() сначала попадает в %eax
    // а только потом копируется в переменную pid, эта
    // последовательность неатомарна
    asm("int $0x80\n" : "=a" (pid) : "a"(__NR_fork));
    if (!pid) {
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "error: %s\n", strerror(errno));
        _exit(1);
    }

    wait(NULL);
}
