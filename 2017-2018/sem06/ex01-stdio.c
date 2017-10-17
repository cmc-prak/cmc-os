#include <stdio.h>
#include <ctype.h>

/*
 * эта программа подсчитывает число цифровых символов в файле
 */
int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    int c;
    int count = 0;
    while ((c = getc_unlocked(f)) != EOF) {
        if (c >= '0' && c <= '9') ++count;
        //if (isdigit(c)) ++count;
    }
    printf("%d\n", count);
}
