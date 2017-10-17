#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd = open("/dev/urandom", O_RDONLY, 0);
    for (int i = 0; i < 10; ++i) {
        unsigned val;
        read(fd, &val, sizeof(val));
        printf("%u\n", val);
    }
}
