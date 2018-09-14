#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char c;
    int r;
    while (1) {
        r = read(STDIN_FILENO, &c, sizeof(c));
        if (r != sizeof(c)) break;
        r = write(STDOUT_FILENO, &c, sizeof(c));
        if (r != 1) abort();
    }
}
