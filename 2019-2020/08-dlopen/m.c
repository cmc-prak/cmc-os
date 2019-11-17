#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <limits.h>

#include "r.h"

typedef struct RandomContext * (*create_func_t)(void);

struct RandomContext *random_factory(const char *name)
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "./rand_%s.so", name);
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "error: %s\n", dlerror());
        exit(1);
    }

    char fn[512];
    snprintf(fn, sizeof(fn), "create_%s_random_v%d", name, INTERFACE_VERSION);
    void *func = dlsym(handle, fn);
    if (!func) {
        fprintf(stderr, "error: %s\n", dlerror());
        exit(1);
    }
    return ((create_func_t) func)();
}

int main(int argc, char *argv[])
{
    int max_rand = strtol(argv[1], NULL, 10);
    int count = strtol(argv[2], NULL, 10);

    struct RandomContext *cntx = random_factory(argv[3]);

    cntx->ops->init(cntx);

    for (int i = 0; i < count; ++i) {
        printf("%d\n", cntx->ops->rand_n(cntx, max_rand));
    }

    cntx->ops->destroy(cntx);
}
