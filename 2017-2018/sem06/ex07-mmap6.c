#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/*
Приватное анонимное отображение - просто память,
выделенная процессу
*/

int main(int argc, char *argv[])
{
    unsigned char *ptr = mmap(NULL, getpagesize() * 256, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    int count = 0;
    int size = getpagesize() * 16;
    for (int i = 0; i < size; ++i) {
        if (ptr[i] >= '0' && ptr[i] <= '9') {
            ptr[i] = 'a';
            ++count;
        }
    }
    printf("%d\n", count);
}
