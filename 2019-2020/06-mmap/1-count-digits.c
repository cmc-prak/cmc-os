#include <stdio.h>

int main()
{
    int c;
    long long count[10] = {};
    while ((c = getchar_unlocked()) != EOF) {
        if (c >= '0' && c <= '9') {
            ++count[c - '0'];
        }
    }
    for (int i = 0; i < 10; ++i) {
        printf("%lld\n", count[i]);
    }
}
