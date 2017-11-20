#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void handler(int s)
{
    fprintf(stderr, "signal: %d\n", s);
}

/*
Использование системного вызова sigaction
 */
int main()
{
    int a, b;
    struct sigaction safpe =
    {
        .sa_handler = handler,
        .sa_flags = SA_RESTART
    };
    sigaction(SIGINT, &safpe, NULL);
    sigaction(SIGFPE, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
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
