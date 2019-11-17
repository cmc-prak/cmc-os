#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

void work(const char *dir)
{
    DIR *d = opendir(dir);
    if (!d) {
        fprintf(stderr, "%s: %s\n", dir, strerror(errno));
        return;
    }

    struct dirent *dd;
    errno = 0;
    while ((dd = readdir(d)) != NULL) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, ".."))
            continue;
        printf("%lu %s\n", dd->d_ino, dd->d_name);

        char *buf = NULL;

        asprintf(&buf, "%s/%s", dir, dd->d_name);

        struct stat sb;
        if (lstat(buf, &sb) < 0) {
            fprintf(stderr, "%s: %s\n", buf, strerror(errno));
        } else {
            printf("dev, ino: %lx %lu\n", sb.st_dev, sb.st_ino);
            if (S_ISDIR(sb.st_mode)) {
                work(buf);
            }
        }
        free(buf);
    }
    if (errno) {
        fprintf(stderr, "%s: read error: %s\n", dir, strerror(errno));
    }

    closedir(d);
}

int main(int argc, char *argv[])
{
    work(argv[1]);
}
