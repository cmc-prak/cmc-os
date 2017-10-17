#include "randgen.h"
#include <stdio.h>
#include <dlfcn.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    //struct RandomState *state = new_prng_gen(argv[1]);
    //void *handle = dlopen(argv[2], 
    unsigned char pname[PATH_MAX];
    snprintf(pname, sizeof(pname), "./rand_%s.so", argv[2]);
    void *handle = dlopen(pname, RTLD_LAZY);
    unsigned char fname[PATH_MAX];
    snprintf(fname, sizeof(fname), "new_%s", argv[2]);
    struct RandomState *(*new_func)(const char *) = dlsym(handle, fname);
    struct RandomState *state = new_func(argv[1]);

    for (int i = 0; i < 10; ++i) {
        double x = state->ops->nextDouble(state);
        printf("%.10g\n", x);
    }
    state->ops->destroy(state);
}
