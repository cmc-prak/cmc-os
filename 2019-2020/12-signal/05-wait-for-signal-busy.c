#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

volatile sig_atomic_t int_flag = 0;

void hand_int(int s)
{
    int_flag = 1;
}

int main()
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = hand_int, .sa_flags = SA_RESTART }, NULL);

    while (1) {
        // BUSY WAIT
        // недопустим в программах, это ошибка!
        while (!int_flag) {}
        int_flag = 0;
        printf("SIGINT\n");
    }
}
