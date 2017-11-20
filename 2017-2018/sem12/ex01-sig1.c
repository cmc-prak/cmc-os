#include <stdio.h>
#include <signal.h>

/*
Игнорируем сигналы SIGINT и SIGFPE.
При выполнении деления на 0 сигнал SIGFPE будет обработан по умолчанию.
При посылке SIGFPE из другого процесса он будет проигнорирован.
 */
int main()
{
    int a, b;
    signal(SIGINT, SIG_IGN);
    signal(SIGFPE, SIG_IGN);
    while (scanf("%d%d", &a, &b) == 2) {
        printf("%d\n", a / b);
    }
}
