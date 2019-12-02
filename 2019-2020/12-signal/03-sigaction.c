#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void hand_int(int s)
{
    printf("SIGINT\n");
}

int main()
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = hand_int, .sa_flags = SA_RESTART }, NULL);

    int a, b;
    while (scanf("%d%d", &a, &b) == 2) {
        int c = a / b;
        printf("%d\n", c);
    }
}
