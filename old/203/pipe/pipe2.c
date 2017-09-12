#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int prevfd = -1;
    for (int i = 1; i < argc; ++i) {
        int fds[2];
        if (i < argc - 1) {
            pipe(fds);
        }
        if (!fork()) {
            if (i > 1) {
                dup2(prevfd, 0);
                close(prevfd);
            }
            if (i < argc - 1) {
                dup2(fds[1], 1);
                close(fds[1]);
                close(fds[0]);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        if (i < argc - 1) {
            close(fds[1]);
        }
        if (i > 1) {
            close(prevfd);
        }
        prevfd = fds[0];
    }
    while (wait(NULL) > 0) {}
}
