#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "r.h"

struct StdlibRandomContext
{
    struct RandomContext b;

    unsigned int seed;
};

static void rand_init(struct RandomContext *cntx)
{
    struct StdlibRandomContext *rc = (struct StdlibRandomContext *) cntx;
    rc->seed = time(NULL);
}

static int rand_n(struct RandomContext *cntx, int n)
{
    struct StdlibRandomContext *rc = (struct StdlibRandomContext *) cntx;
    assert(n > 0);
    int res = (int)((rand_r(&rc->seed) / (RAND_MAX + 1.0)) * n);
    return res;
}

static void rand_destroy(struct RandomContext *cntx)
{
    struct StdlibRandomContext *rc = (struct StdlibRandomContext *) cntx;
    free(rc);
}

static const struct RandomOperations ops =
{
    rand_init,
    rand_destroy,
    rand_n
};

struct RandomContext *
CREATE_FUNC_NAME(stdlib,1)()
{
    struct StdlibRandomContext *rc = calloc(1, sizeof(*rc));
    rc->b.ops = &ops;
    return &rc->b;
}
