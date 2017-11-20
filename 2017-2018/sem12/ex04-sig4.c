#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void handler(int s)
{
    // в обработчиках сигнала можно безопасно
    // использовать только async-signal safe функции
    static const char msg[] = "signal whatever\n";
    // даже в случае async-signal safe функций
    // нужно сохранить и восстановить переменную errno
    int err = errno;
    write(2, msg, sizeof(msg) - 1);
    errno = err;
}

int main()
{
    sigset_t s1, s2;

    // блокируем SIGINT, SIGKILL не может быть заблокирован
    sigemptyset(&s1);
    sigaddset(&s1, SIGINT);
    sigaddset(&s1, SIGKILL);
    sigprocmask(SIG_BLOCK, &s1, &s2);

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
        if (b == 1) {
            // восстанавливаем исходное множество блокируемых сигналов,
            // то есть разблокируем SIGINT
            sigprocmask(SIG_SETMASK, &s2, NULL);
        } else if (b == 2) {
            // снова блокируем SIGINT
            sigprocmask(SIG_BLOCK, &s1, NULL);
        }
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
