#include "randint.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <limits.h>

typedef struct RandomState * (*init_func_t)(char const *);

int main(int argc, char *argv[])
{
    struct RandomState *state = NULL;

    char fname[PATH_MAX];
    snprintf(fname, sizeof(fname), "./rand_%s.so", argv[2]);
    //void *handle = dlopen(fname, rtld_lazy);
    void *handle = dlopen(NULL, RTLD_LAZY);
    char sname[PATH_MAX];
    snprintf(sname, sizeof(sname), "init_%s_gen", argv[2]);
    init_func_t sym = dlsym(handle, sname);

    //state = ((struct RandomState * (*)(char const *)) sym)(argv[1]);
    state = sym(argv[1]);
    // init state
    //state = init_prng_gen(argv[1]);
    //state = init_trng_gen(argv[1]);
    for (int i = 0; i < 10; ++i) {
        double x = state->ops->next(state);
        printf("%.10g\n", x);
    }
    state->ops->destroy(state);
    dlclose(handle);
}
