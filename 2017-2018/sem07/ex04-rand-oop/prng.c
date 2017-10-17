#include "randint.h"
#include <stdlib.h>

struct PrngState
{
    struct RandomState b;
    unsigned int seed;
};

static void destroy(struct RandomState *rst)
{
    free(rst);
}

static double next(struct RandomState *rst)
{
    struct PrngState *pst = (struct PrngState *) rst;
    return rand_r(&pst->seed) / (RAND_MAX + 1.);
}

static const struct RandomOps ops =
{
    destroy,
    next
};

struct RandomState *init_prng_gen(const char *str)
{
    struct PrngState *pst = calloc(1, sizeof(*pst));
    pst->b.ops = &ops;
    pst->seed = strtoul(str, NULL, 10);
    return &pst->b;
}

