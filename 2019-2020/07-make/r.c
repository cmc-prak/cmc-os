#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "r.h"

int rand_init()
{
    srand(time(NULL));
    return 0;
}

int rand_n(int n)
{
    assert(n > 0);
    return (int)((rand() / (RAND_MAX + 1.0)) * n);
}
