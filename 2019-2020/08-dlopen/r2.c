#include "r.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

struct UrandomRandomContext
{
    struct RandomContext b;
    int rand_fd;
};

static void rand_init(struct RandomContext *cntx)
{
    struct UrandomRandomContext *rc = (struct UrandomRandomContext *) cntx;
    rc->rand_fd = open("/dev/urandom", O_RDONLY);
    assert(rc->rand_fd >= 0);
}

static int rand_n(struct RandomContext *cntx, int n)
{
    struct UrandomRandomContext *rc = (struct UrandomRandomContext *) cntx;
    unsigned val;
    read(rc->rand_fd, &val, sizeof(val));
    return val % n;
}

static void rand_destroy(struct RandomContext *cntx)
{
    struct UrandomRandomContext *rc = (struct UrandomRandomContext *) cntx;
    close(rc->rand_fd);
    free(rc);
}

static const struct RandomOperations ops =
{
    rand_init,
    rand_destroy,
    rand_n
};

struct RandomContext *
create_urandom_random()
{
    struct UrandomRandomContext *rc = calloc(1, sizeof(*rc));
    rc->b.ops = &ops;
    rc->rand_fd = -1;
    return &rc->b;
}
