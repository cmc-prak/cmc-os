#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

void dowork(int id, int fd0, int fd1)
{
    while (1) {
        int x;
        read(fd0, &x, sizeof(x));
        printf("%d %d\n", id, x);
        fflush(stdout);
        ++x;
        write(fd1, &x, sizeof(x));
    }
}

int main(void)
{
    int fds4[2];
    int fds7[2];
    pipe(fds4);
    pipe(fds7);
    if (!fork()) {
        dowork(1, fds4[0], fds7[1]);
    }
    if (!fork()) {
        dowork(2, fds7[0], fds4[1]);
    }
    int x = 1;
    write(fds4[1], &x, sizeof(x));
    wait(NULL);
    wait(NULL);
}
