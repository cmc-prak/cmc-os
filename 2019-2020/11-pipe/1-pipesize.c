#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int main()
{
    char c;
    int z = 0;
    int fd[2];
    pipe(fd);
    close(fd[0]);
    signal(SIGPIPE, SIG_IGN);
    while (1) {
        int r = write(fd[1], &c, sizeof(c));
        if (r > 0) {
            ++z;
            printf("%d\n", z);
        } else if (!r) {
            printf("EOF\n");
        } else {
            printf("%s\n", strerror(errno));
            return 1;
        }
    }
}
