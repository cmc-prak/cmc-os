#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    //int fd = open("/dev/random", O_RDONLY, 0);
    // системный генератор случайных чисел
    int fd = open("/dev/urandom", O_RDONLY, 0);
    for (int i = 0; i < 10; ++i) {
        unsigned val;
        read(fd, &val, sizeof(val));
        double x = val / (UINT_MAX + 1.);
        printf("%u %.10g\n", val, x);
    }
    close(fd);
}
