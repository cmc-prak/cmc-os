#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *path = argv[1];

    int fd = open(path, O_RDONLY, 0);
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

    if (!sb.st_size) {
        return 0;
    }

    ssize_t size = sb.st_size;
    if (size != sb.st_size) {
        fprintf(stderr, "%s: file too big\n", path);
        exit(1);
    }

    unsigned char *ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));
        exit(1);
    }
    close(fd);

    long long count[10] = {};
    const unsigned char *eptr = ptr + size;
    for (const unsigned char *p = ptr; p < eptr; ++p) {
        unsigned char tmp = *p - '0';
        if (tmp <= 9) {
            ++count[tmp];
        }
    }

    munmap(ptr, size);
    for (int i = 0; i < 10; ++i) {
        printf("%lld\n", count[i]);
    }
}
