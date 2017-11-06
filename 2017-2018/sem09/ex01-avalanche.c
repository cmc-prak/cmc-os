#include <stdio.h>
#include <unistd.h>

// сколько будет процессов выполнять pause()?
int main()
{
    fork();
    fork();
    printf("%d\n", getpid());
    pause();
}
