#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/*
Различие SystemV и BSD семантики обработки сигналов.
BSD семантика:
gcc ex01-sig2.c -O2 -Wall -std=gnu11 -oex01-sig2
SystemV семантика
gcc ex01-sig2.c -O2 -Wall -std=c11 -D_POSIX_C_SOURCE=299901 -oex01-sig2
 */
void handler(int s)
{
    fprintf(stderr, "signal: %d\n", s);
}

int main()
{
    int a, b;
    signal(SIGINT, handler);
    signal(SIGFPE, handler);
    int r;
restart:
    while ((r = scanf("%d%d", &a, &b)) == 2) {
        printf("%d\n", a / b);
    }
    if (r == EOF) {
        if (feof(stdin)) {
            printf("EOF!\n");
        } else if (ferror(stdin)) {
            printf("%s\n", strerror(errno));
            goto restart;
        }
    }
}
