#include <stdio.h>
#include <stdlib.h>

#include "r.h"

int main(int argc, char *argv[])
{
    int max_rand = strtol(argv[1], NULL, 10);
    int count = strtol(argv[2], NULL, 10);

    rand_init();

    for (int i = 0; i < count; ++i) {
        printf("%d\n", rand_n(max_rand));
    }
}
