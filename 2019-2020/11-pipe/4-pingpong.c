#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void work(int rfd, int wfd, int maxval)
{
    while (1) {
        int v;
        int r = read(rfd, &v, sizeof(v));
        if (r <= 0) break;
        ++v;
        printf("%d %d\n", getpid(), v - 1); fflush(stdout);
        if (v == maxval) {
            //close(wfd);
            break;
        }
        write(wfd, &v, sizeof(v));
    }
}

int main()
{
    int fd12[2];
    int fd21[2];

    pipe(fd12);
    pipe(fd21);

    if (!fork()) {
        close(fd12[0]); close(fd21[1]);
        work(fd21[0], fd12[1], 101);
        _exit(1);
    }
    if (!fork()) {
        close(fd12[1]); close(fd21[0]);
        work(fd12[0], fd21[1], 101);
        _exit(1);
    }

    int v = 1;
    write(fd21[1], &v, sizeof(v));

    close(fd12[0]); close(fd21[1]);
    close(fd12[1]); close(fd21[0]);
    while (wait(NULL) > 0) {}
}
