#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

enum { PORTNUM = 11111 };

int main(void)
{
    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        return 1;
    }

    int value = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value));

    struct sockaddr_in s1;
    s1.sin_family = AF_INET;
    s1.sin_port = htons(PORTNUM);
    s1.sin_addr.s_addr = INADDR_ANY;
    int r = bind(fd, (struct sockaddr *) &s1, sizeof(s1));
    if (r < 0) {
        fprintf(stderr, "bind: %s\n", strerror(errno));
        return 1;
    }

    listen(fd, 5);

    while (1) {
        struct sockaddr_in s2;
        socklen_t slen = sizeof(s2);
        int afd = accept(fd, (struct sockaddr *) &s2, &slen);
        if (afd < 0) {
            fprintf(stderr, "accept: %s\n", strerror(errno));
            return 1;
        }

        if (!fork()) {
            close(fd);
            char c;
            while (read(afd, &c, sizeof(c)) == 1) {
                write(1, &c, sizeof(c));
                if (isdigit(c))
                    ++c;
                write(afd, &c, sizeof(c));
            }
            _exit(0);
        }
        close(afd);
    }
}
