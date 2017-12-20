#include <stdint.h>
#include <limits.h>

enum { N = 1024 };

uint8_t bits[N];

// получить бит с индексом i в битовом массиве bits
int get_bit(unsigned i)
{
    return (bits[i >> 3] >> (i & 7)) & 1;
}
