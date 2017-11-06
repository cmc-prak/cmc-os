#include <stdio.h>
#include <unistd.h>

// буферизация стандартных потоков
int main()
{
    printf("Hello"); fflush(stdout);
    //fprintf(stderr, "Hello");
    //setbuf(stdout, NULL);
    printf("Hello");
    fork();
}
