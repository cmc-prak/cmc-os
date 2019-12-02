#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void hand_int(int s)
{
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

    int a, b, r;
    while ((r = scanf("%d%d", &a, &b)) == 2) {
        int c = a / b;
        printf("%d\n", c);
    }
}
