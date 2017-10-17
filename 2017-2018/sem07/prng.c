#include "randgen.h"
#include <stdlib.h>

struct PrngState
{
    struct RandomState b;
    unsigned int seed;
};

static void destroy_func(struct RandomState *rs)
{
    struct PrngState *fs = (struct PrngState *) rs;
    free(fs);
}

static double nextDouble_func(struct RandomState *rs)
{
    struct PrngState *fs = (struct PrngState *) rs;
    int val = rand_r(&fs->seed);
    return val / (RAND_MAX + 1.);
}

static const struct RandomOps ops =
{
    destroy_func,
    nextDouble_func
};

struct RandomState *new_prng_gen(const char *init_data)
{
    struct PrngState *fs = calloc(1, sizeof(*fs));
    fs->b.ops = &ops;
    fs->seed = strtoul(init_data, NULL, 10);
    return &fs->b;
}
