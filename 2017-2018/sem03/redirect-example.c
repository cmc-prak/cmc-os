/*
 * В этом примере стандартный поток вывода перенаправляется в файл за счет того,
 * что номер закрытого файлового дескриптора (1) повторно используется в open.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
    close(1);
    int old = umask(0);
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    printf("Hello %d\n", fileno(stdout));
}
