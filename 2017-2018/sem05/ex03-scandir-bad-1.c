#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * В этом примере _неправильно_ обрабатывается содержимое каталога
 */

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    if (!d) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    struct dirent *dd;
    while ((dd = readdir(d))) {
        printf("%lu %s\n", dd->d_ino, dd->d_name);
        struct stat stb;
        if (stat(dd->d_name, &stb) < 0) continue;
        //       ^^^^^^^^^^ - здесь ошибка: должен быть полный путь, а не последняя компонента пути
        printf("stat: %lu %llu\n", stb.st_ino, stb.st_dev);
    }
    closedir(d);
}
