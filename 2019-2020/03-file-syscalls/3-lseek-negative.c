#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    int count = strtol(argv[2], NULL, 10);
    long long val1 = strtoll(argv[3], NULL, 10);
    long long val2 = strtoll(argv[4], NULL, 10);

    lseek(fd, (count - 1) * sizeof(val1), SEEK_SET);
    for (; count; --count) {
        write(fd, &val1, sizeof(val1));
        int64_t tmp = val1;
        val1 = val2;
        val2 += tmp;
        lseek(fd, -2LL * sizeof(val1), SEEK_CUR);
    }
    close(fd);
}
