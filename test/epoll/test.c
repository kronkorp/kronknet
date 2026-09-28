#include <cstddef>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 1024

int main(
    int argc,
    char *const *argv
)
{

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {};

    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(fd);
        return 84;
    }

    if (listen(fd, SOMAXCONN) == -1) {
        close(fd);
        return 84;
    }

    int epollfd = epoll_create1(0);

    if (epollfd == -1) {
        return 84;
    }

    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = fd;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, events) == -1) {
        close(fd);
        close(epollfd);
    }

    for (;;) {
        ssize_t ndfs = epoll_wait(epollfd, events, MAX_EVENTS, -1);

        if (ndfs == -1) {
            close(fd);
            close(epollfd);
            return 84;
        }

        for (ssize_t n = 0; n < ndfs; ++n) {
            if (events[n].data.fd == fd) {
                
            }
        }
    }

}
