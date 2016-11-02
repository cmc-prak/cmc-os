#include <stdlib.h>

typedef struct Random Random;

int next42(Random *r) {
    return 42;
}

Random *destroy42(Random *r) {
    free(r);
    return NULL;
}
