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
 * Использование нестандартной функции asprintf (GNU extension)
 * Компилировать с опцией -D_GNU_SOURCE
 */

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    if (!d) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    struct dirent *dd;
    char *path = NULL;
    while ((dd = readdir(d))) {
        free(path);
        int slen = asprintf(&path, "%s/%s", argv[1], dd->d_name);

        printf("%s %d %lu %s\n", path, slen, dd->d_ino, dd->d_name);
        struct stat stb;
        if (stat(path, &stb) < 0) continue;
        printf("stat: %lu %llu\n", stb.st_ino, stb.st_dev);
    }
    free(path);
    closedir(d);
}
