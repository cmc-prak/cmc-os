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

    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        exit(1);
    }

    struct stat sb;
    fstat(fd, &sb);
    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "%s: not regular\n", path);
        exit(1);
    }

    off_t fsize = (off_t) count * (off_t) sizeof(ValueType);

    if (ftruncate(fd, fsize) < 0) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        exit(1);
    }

    ValueType *ptr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        exit(1);
    }
    close(fd);

    for (int i = 1; i <= count; ++i) {
        ptr[count - i] = i;
    }

    munmap(ptr, fsize);
}
