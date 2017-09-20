#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    off_t res = lseek(1, 20, SEEK_END);
    if (res < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    char c = '?';
    int r = write(1, &c, 1);
    printf("%d\n", r);
    putchar(c);
    putchar('\n');
}
