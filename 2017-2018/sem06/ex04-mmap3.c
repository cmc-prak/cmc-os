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
Память отображается страницами (x86 - 4KiB), поэтому
последняя страница будет доступна как полная,
однако изменения на последней странице за текущим концом
файла не будут записаны в файл.
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
    int count = 0;
    for (int i = 0; i < 4096; ++i) {
        if (ptr[i] >= '0' && ptr[i] <= '9') {
            ptr[i] = 'a';
            ++count;
        } else if (!ptr[i]) {
            ptr[i] = 'x';
        }
    }
    printf("%d\n", count);
    ftruncate(fd, 1024); // меняем размер файла
    close(fd);
}
