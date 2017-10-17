#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum { A = 1103515245U, C = 12345U, M = 1U << 31 };

int main()
{
    int seed = time(NULL);
    unsigned cur = seed;
    srand(seed);
    for (int i = 0; i < 10; ++i) {
        cur = (cur * A + C);// % M;
        int x = rand();
        printf("%d %u %d\n", x, cur, RAND_MAX); // [0; RAND_MAX]
        // [0;1)
        double y = x / (RAND_MAX + 1.);
        printf("%.10g\n", y);
        // [LOW; HIGH)
        enum { LOW = -10, HIGH = -8 };
        //int z = LOW + x % (HIGH - LOW); // not so random
        //printf("%d\n", z);
        int zz = LOW + (int)(y * (HIGH - LOW)); // good!
        printf("%d\n", zz);
    }
}
