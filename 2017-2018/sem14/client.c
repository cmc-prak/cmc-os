#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

enum { PORTNUM = 11111 };

int main(int argc, char *argv[])
{
    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        return 1;
    }

    int port = strtol(argv[1], 0, 10);

    struct sockaddr_in s1;
    inet_aton(argv[2], &s1.sin_addr);
    s1.sin_family = AF_INET;
    s1.sin_port = htons(port);
    if (connect(fd, (struct sockaddr *) &s1, sizeof(s1)) < 0) {
        fprintf(stderr, "connect: %s\n", strerror(errno));
        return 1;
    }

    char c;
    while (read(0, &c, sizeof(c)) == 1) {
        write(fd, &c, sizeof(c));
        read(fd, &c, sizeof(c));
        write(1, &c, sizeof(c));
    }
}
