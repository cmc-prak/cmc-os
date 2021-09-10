#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argv[1]) {
        const char *s = getenv(argv[1]);
        if (s) {
            printf("%s\n", s);
        } else {
            fprintf(stderr, "%s: variable '%s' is undefined\n", argv[0], argv[1]);
            return 1;
        }
    }
}
