#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
 * эта программа выводит содержимое каталога в виде последовательности пар Inode:Name
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
    }
    closedir(d);
}
