#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);

    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }

}
