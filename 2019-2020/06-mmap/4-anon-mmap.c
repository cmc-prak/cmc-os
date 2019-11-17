#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long long ValueType;

int main(int argc, char *argv[])
{
    char *path = argv[1];
    int count = strtol(argv[2], NULL, 10);

    off_t fsize = (off_t) count * (off_t) sizeof(ValueType);

    ValueType *ptr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        exit(1);
    }

    printf("%p\n", ptr);
    getchar();

    for (int i = 1; i <= count; ++i) {
        ptr[count - i] = i;
    }

    munmap(ptr, fsize);
}
