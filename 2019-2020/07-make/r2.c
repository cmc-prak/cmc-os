#include "r.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

static int rand_fd = -1;

int rand_init()
{
    rand_fd = open("/dev/urandom", O_RDONLY);
    if (rand_fd < 0) return -1;
    return 0;
}

int rand_n(int n)
{
    unsigned val;
    read(rand_fd, &val, sizeof(val));
    return val % n;
}
