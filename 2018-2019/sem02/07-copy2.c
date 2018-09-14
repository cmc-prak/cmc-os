#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum { BUFSIZE = 4096 };

int main()
{
    char buf[BUFSIZE];
    ssize_t r, w;
    while (1) {
        r = read(STDIN_FILENO, &buf, sizeof(buf));
        if (r < 0) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            exit(1);
        }
        if (!r) break;
        char *p = buf;
        while (r > 0) {
            w = write(STDOUT_FILENO, p, r);
            if (w < 0) {
                fprintf(stderr, "error: %s\n", strerror(errno));
                exit(1);
            }
            r -= w;
            p += w;
        }
    }
}
