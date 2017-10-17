#include "randgen.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

struct PrngState
{
    struct RandomState b;
    int fd;
};

static void destroy_func(struct RandomState *rs)
{
    struct PrngState *fs = (struct PrngState *) rs;
    close(fs->fd);
    free(fs);
}

static double nextDouble_func(struct RandomState *rs)
{
    struct PrngState *fs = (struct PrngState *) rs;
    unsigned val;
    read(fs->fd, &val, sizeof(val));
    return val / (UINT_MAX + 1.0);
}

static const struct RandomOps ops =
{
    destroy_func,
    nextDouble_func
};

struct RandomState *new_urandom_gen(const char *init_data)
{
    struct PrngState *fs = calloc(1, sizeof(*fs));
    fs->b.ops = &ops;
    fs->fd = open("/dev/urandom", O_RDONLY, 0);
    return &fs->b;
}
