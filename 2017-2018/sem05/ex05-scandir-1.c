#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

/*
 * Использование snprintf для получения полного пути
 */

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    if (!d) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    struct dirent *dd;
    char path[PATH_MAX];
    while ((dd = readdir(d))) {
        int slen = snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);
        // в переменной slen находится длина строки в предположении, что буфер неограничен
        if (slen + 1 > sizeof(path)) {
            // буфер оказался недостаточного размера для размещения всего пути
            // как-то обработать эту ситуацию...
            continue;
        }

        printf("%s %d %lu %s\n", path, slen, dd->d_ino, dd->d_name);
        struct stat stb;
        if (stat(path, &stb) < 0) continue;
        printf("stat: %lu %llu\n", stb.st_ino, stb.st_dev);
    }
    closedir(d);
}
