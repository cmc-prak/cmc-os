#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

enum { MAXVAL = 10000 };

void work(int rfd, int wfd)
{
    int x;
    while (read(rfd, &x, sizeof(x)) == sizeof(x)) {
        ++x;
        printf("%d %d\n", getpid(), x - 1); fflush(stdout);
        if (x == MAXVAL) break;
        write(wfd, &x, sizeof(x));
    }
}

int main()
{
    int p12[2];
    int p21[2];

    pipe(p12);
    pipe(p21);

    if (!fork()) {
        close(p21[1]); close(p12[0]);
        work(p21[0], p12[1]);
        _exit(1);
    }
    if (!fork()) {
        close(p12[1]); close(p21[0]);
        work(p12[0], p21[1]);
        _exit(1);
    }
    int z = 1;
    write(p21[1], &z, sizeof(z));

    close(p12[0]); close(p12[1]);
    close(p21[0]); close(p21[1]);
    wait(NULL);
    wait(NULL);
}
