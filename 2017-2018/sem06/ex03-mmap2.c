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
модификация файла, отображенного в память
 */

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR, 0);
    if (fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return 1;
    }
    struct stat stb;
    fstat(fd, &stb);
    unsigned char *ptr = mmap(NULL, stb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    close(fd);
    int count = 0;
    for (int i = 0; i < stb.st_size; ++i) {
        if (ptr[i] >= '0' && ptr[i] <= '9') {
            ptr[i] = 'a';
            ++count;
        }
    }
    printf("%d\n", count);
}
