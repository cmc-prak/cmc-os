#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// определение размера буфера канала
int main()
{
    int fd[2];

    pipe(fd);
    int sz = 0;
    char c = 'a';
    while (1) {
        printf("%d\n", sz);
        ssize_t r = write(fd[1], &c, sizeof(c));
        if (r < 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(1);
        }
        ++sz;
    }
}
