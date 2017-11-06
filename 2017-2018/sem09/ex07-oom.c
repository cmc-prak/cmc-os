#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { SZ = 1 * 1024 * 1024 * 1024U + 512 * 1024 * 1024U };

// из-за Copy-on-Write и Memory Overcommit
// возникает ситуация Out-of-Memory и ядро ОС
// завершает процессы, отправляя им SIGKILL
int main()
{
    unsigned char *ptr = malloc(SZ);
    fork();
    fork();
    memset(ptr, 1, SZ);
    printf("%d %d\n", ptr[SZ - 1], getpid());
}
