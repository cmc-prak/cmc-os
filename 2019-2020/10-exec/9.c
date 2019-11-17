#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    uid_t uid = getuid();
    uid_t euid = geteuid();

    printf("%d %d\n", uid, euid);

    FILE *f = fopen(argv[1], "r");
    if (f) {
        int c;
        while ((c = getc(f)) != EOF) putchar(c);
    } else {
        fprintf(stderr, "error: %s\n", strerror(errno));
    }
}
