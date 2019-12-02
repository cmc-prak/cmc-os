#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void hand_int(int s)
{
    static const char buf[] = "SIGINT\n";
    int e = errno;
    write(1, buf, sizeof(buf) - 1);
    errno = e;
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
