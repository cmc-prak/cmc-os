#include <unistd.h>

int main()
{
    static const char msg[] = "Hello!\n";

    write(1, msg, sizeof(msg) - 1);
    _exit(0);
}
