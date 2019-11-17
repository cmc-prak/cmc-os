#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

int main()
{
    //char buf[256];
    //setbuffer(stdout, buf, sizeof(buf));
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        exit(1);
    } else if (!pid) {
        printf("%d %d\n", getpid(), getppid());
        char *p = (char*) (intptr_t) getpid();
        printf("%c\n", *p);
        abort();
        return 111;
        exit(42);
    } else {
        int status = 0;
        struct rusage ru;
        pid_t p = wait4(-1, &status, 0, &ru);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("ok %d %lld\n", p, ru.ru_utime.tv_sec * 100000LL + ru.ru_utime.tv_usec);
        } else if (WIFEXITED(status)) {
            printf("exit %d %d %lld\n", WEXITSTATUS(status), p, ru.ru_utime.tv_sec * 100000LL + ru.ru_utime.tv_usec);
        } else if (WIFSIGNALED(status)) {
            printf("signal %d %d %lld\n", WTERMSIG(status), p, ru.ru_utime.tv_sec * 100000LL + ru.ru_utime.tv_usec);
        }
        printf("%d %d\n", getpid(), pid);
    }
}
