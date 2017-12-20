#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/signalfd.h>

enum { PORTNUM = 11111 };

int main(void)
{
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigprocmask(SIG_BLOCK, &ss, NULL);

    int sfd = signalfd(-1, &ss, 0);

    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        return 1;
    }

    int value = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

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

    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

    int efd = epoll_create1(0);

    epoll_ctl(efd, EPOLL_CTL_ADD, fd, &(struct epoll_event) { .events = EPOLLIN, .data.fd = fd });
    epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &(struct epoll_event) { .events = EPOLLIN, .data.fd = sfd });

    while (1) {
        enum { EVENT_SIZE = 5 };
        struct epoll_event events[EVENT_SIZE];
        int r = epoll_pwait(efd, events, EVENT_SIZE, 1000, NULL);
        if (r < 0) {
            fprintf(stderr, "epoll: %s\n", strerror(errno));
            return 1;
        }
        if (!r) {
            printf("timeout!\n");
            continue;
        }

        for (int i = 0; i < r; ++i) {
            struct epoll_event *pe = &events[i];

            if (pe->events == EPOLLIN && pe->data.fd == fd) {

                struct sockaddr_in s2;
                socklen_t slen = sizeof(s2);
                int afd = accept(fd, (struct sockaddr *) &s2, &slen);
                if (afd < 0) {
                    fprintf(stderr, "accept: %s\n", strerror(errno));
                    return 1;
                }
                fcntl(afd, F_SETFL, fcntl(afd, F_GETFL) | O_NONBLOCK);
                epoll_ctl(efd, EPOLL_CTL_ADD, afd, &(struct epoll_event) { .events = EPOLLIN, .data.fd = afd });
            } else if (pe->events == EPOLLIN && pe->data.fd == sfd) {
                struct signalfd_siginfo si;
                read(sfd, &si, sizeof(si));
                printf("%d\n", si.ssi_signo);

                for (int f = 3; f < 250; ++f) {
                    if (f != sfd && f != fd && f != efd) {
                        if (fcntl(f, F_GETFL, 0) >= 0) {
                            epoll_ctl(efd, EPOLL_CTL_DEL, f, NULL);
                            close(f);
                        }
                    }
                }


            } else if (pe->events == EPOLLIN) {
                int afd = pe->data.fd;
                char c;
                r = read(afd, &c, sizeof(c));
                if (r < 0) {
                    fprintf(stderr, "read: %s\n", strerror(errno));
                    return 1;
                } else if (!r) {
                    epoll_ctl(efd, EPOLL_CTL_DEL, afd, NULL);
                    close(afd);
                } else {
                    write(1, &c, sizeof(c));
                    if (isdigit(c))
                        ++c;
                    write(afd, &c, sizeof(c));
                }
            }
        }
    }
}
