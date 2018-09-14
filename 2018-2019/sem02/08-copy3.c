#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { BUFSIZE = 4096 };

int main(int argc, char *argv[])
{
    const char *infile = argv[1];
    const char *outfile = argv[2];

    int rfd = open(infile, O_RDONLY, 0);
    if (rfd < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    int wfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (wfd < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }

    char buf[BUFSIZE];
    ssize_t r, w;
    while (1) {
        r = read(rfd, &buf, sizeof(buf));
        if (r < 0) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            exit(1);
        }
        if (!r) break;
        char *p = buf;
        while (r > 0) {
            w = write(wfd, p, r);
            if (w < 0) {
                fprintf(stderr, "error: %s\n", strerror(errno));
                exit(1);
            }
            r -= w;
            p += w;
        }
    }

    close(rfd);
    close(wfd);
}
