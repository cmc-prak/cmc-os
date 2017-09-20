#include <unistd.h>

const char str[] = "Hello\n";
int main()
{
    write(1, str, sizeof(str) - 1);
    _exit(4);
}
