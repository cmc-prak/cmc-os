#include "randint.h"
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

struct TrngState
{
    struct RandomState b;
    int fd;
};

static void destroy(struct RandomState *rst)
{
    struct TrngState *pst = (struct TrngState *) rst;
    close(pst->fd);
    free(rst);
}

static double next(struct RandomState *rst)
{
    struct TrngState *pst = (struct TrngState *) rst;
    unsigned val;
    read(pst->fd, &val, sizeof(val));
    return val / (UINT_MAX + 1.);
}

static const struct RandomOps ops =
{
    destroy,
    next
};

struct RandomState *init_trng_gen(const char *str)
{
    struct TrngState *pst = calloc(1, sizeof(*pst));
    pst->b.ops = &ops;
    pst->fd = open("/dev/urandom", O_RDONLY, 0);
    return &pst->b;
}

