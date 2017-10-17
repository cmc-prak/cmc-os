#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    unsigned seed = strtol(argv[1], NULL, 10);

    // time() может изменяться медленнее, чем требуется
    if (!seed) seed = time(NULL);

    for (int i = 0; i < 10; ++i) {
        int r = rand_r(&seed);
        // преобразуем в вещественное равномерно распределенное [0;1)
        double x = r / (RAND_MAX + 1.);
        // дискретное равномерное [0; 10)
        int a = x * 10;
        // дискретное равномерное [A; B)
        enum { A = -20, B = -10 };
        int b = (int) (x * (B - A)) + A;
        printf("%d %.10g %d %d\n", r, x, a, b);
    } 
}
