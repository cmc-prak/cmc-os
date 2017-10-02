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
 * В этом примере _НЕДОПУСТИМО_ обрабатывается содержимое каталога
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
        strcat(strcat(strcpy(path, argv[1]), "/"), dd->d_name);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // ОЧЕНЬ-ОЧЕНЬ-ОЧЕНЬ плохо. Возможно переполнение буфера!
        // sprintf также плохо
        printf("%s %lu %s\n", path, dd->d_ino, dd->d_name);
        struct stat stb;
        if (stat(path, &stb) < 0) continue;
        printf("stat: %lu %llu\n", stb.st_ino, stb.st_dev);
    }
    closedir(d);
}
