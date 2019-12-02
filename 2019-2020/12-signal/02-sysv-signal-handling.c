#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void hand_int(int s)
{
    signal(SIGINT, hand_int);
    printf("SIGINT\n");
}

void hand_fpe(int s)
{
    printf("SIGFPE\n");
}

int main()
{
    signal(SIGINT, hand_int);
    signal(SIGFPE, hand_fpe);
    //signal(SIGFPE, SIG_IGN);

    int a, b, r;
restart:
    while ((r = scanf("%d%d", &a, &b)) == 2) {
        int c = a / b;
        printf("%d\n", c);
    }
    if (r == EOF && ferror(stdin)) {
        printf("error: %s\n", strerror(errno));
        if (errno == EINTR) goto restart;
    }
}
