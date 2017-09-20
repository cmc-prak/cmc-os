/*
 * Буферизированное чтение со стандартного потока ввода.
 * В считанном буфере отсутствует байт '\0' терминатор строки,
 * об этом нужно заботиться самим.
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
    char buf[32];
    ssize_t rs;
    while ((rs = read(0, buf, sizeof(buf))) > 0) {
        printf("%d %.*s\n", (int) rs, (int) rs, buf);
    }
}
