#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

// определение размера буфера канала (автоматическое)
int main()
{
    int fd[2];

    pipe2(fd, O_NONBLOCK);
    int sz = 0;
    char c = 'a';
    while (1) {
        //printf("%d\n", sz);
        ssize_t r = write(fd[1], &c, sizeof(c));
        if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("%d\n", sz);
            break;
        }
        if (r < 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(1);
        }
        ++sz;
    }
}
