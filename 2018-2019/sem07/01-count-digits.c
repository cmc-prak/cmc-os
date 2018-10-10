#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum { DIGIT_COUNT = '9' - '0' + 1 };

void print_counters(const long long *counts, int size)
{
    for (int i = 0; i < size; ++i) {
        printf("%d %lld\n", i, counts[i]);
    }
}

int main(int argc, char *argv[])
{
    long long counts[DIGIT_COUNT] = {};
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0) {
        fprintf(stderr, "open failed: %s\n", strerror(errno));
        return 1;
    }

    struct stat stb;
    fstat(fd, &stb);  // в каких случаях fstat может вернуть ошибку?

    off_t size = stb.st_size;
    if (!size) { // поведение mmap при файле нулевого размера - UB
        // но файл нулевого размера - это нормально
        print_counters(counts, DIGIT_COUNT);
        close(fd);
        return 0;
    }

    if ((size_t) size != size) {
        // если размер файла непредставим в типе size_t
        fprintf(stderr, "file is too big\n");
        close(fd);
        return 1;
    }

    void *vptr = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (vptr == MAP_FAILED) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    // после mmap файловый дескриптор можно закрыть
    close(fd);

    const unsigned char *cptr = vptr;
    for (size_t i = 0; i < size; ++i) {
        unsigned val = *cptr++ - '0';
        if (val <= 9) {
            ++counts[val];
        }
    }

    print_counters(counts, DIGIT_COUNT);
    munmap(vptr, size);
}
