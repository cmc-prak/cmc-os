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
    int old_mask = umask(0);

    printf("%o\n", old_mask);
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);

    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }

}
