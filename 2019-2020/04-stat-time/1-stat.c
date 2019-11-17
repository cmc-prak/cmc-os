#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int retval = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat sb;
        if (lstat(argv[i], &sb) < 0) {
            fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
            retval = 1;
        } else {
            printf("%s: size: %lld\n", argv[i], (long long) sb.st_size);
            printf("%s: access: %03o\n", argv[i], sb.st_mode & 0777);
            printf("%s: blocks: %lld\n", argv[i], (long long) sb.st_blocks);
            if (S_ISLNK(sb.st_mode)) {
                char lnk[PATH_MAX];
                ssize_t z = readlink(argv[i], lnk, sizeof(lnk));
                printf("%s: symlink: %.*s\n", argv[i], (int) z, lnk);
            }
            struct tm ltm;
            localtime_r(&sb.st_mtime, &ltm);
            printf("%s: mtime: %d-%02d-%02d %02d:%02d:%02d %d %d %d\n", argv[i],
                    ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
                    ltm.tm_hour, ltm.tm_min, ltm.tm_sec,
                    ltm.tm_wday, ltm.tm_yday, ltm.tm_isdst);
        }
    }
    return retval;
}
