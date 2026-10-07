/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Sockets on POSIX.
*/
#include "../socket.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/optimization.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

int knSocket_startup(void)
{
    return KNEVTOK;
}

void knSocket_cleanup(void)
{
}

static int __knSocket_nonBlocking(
    knSocket fd
)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

knSocket knSocket_open(
    int family,
    int type
)
{
    knSocket fd = socket(family, type, 0);

    if (fd == KN_INVALID_SOCKET) {
        return KN_INVALID_SOCKET;
    }
    if (__knSocket_nonBlocking(fd) != KNEVTOK) {
        close(fd);
        return KN_INVALID_SOCKET;
    }
    return fd;
}

knSocket knSocket_accept(
    knSocket listener,
    knAddr *addr
)
{
    knSocket fd;

    addr->len = sizeof(addr->v6);
    fd = accept(listener, &addr->any, &addr->len);

    if (fd == KN_INVALID_SOCKET) {
        return KN_INVALID_SOCKET;
    }
    if (__knSocket_nonBlocking(fd) != KNEVTOK) {
        close(fd);
        return KN_INVALID_SOCKET;
    }
    return fd;
}

void knSocket_close(
    knSocket fd
)
{
    if (fd != KN_INVALID_SOCKET) {
        close(fd);
    }
}

int knSocket_setReuseAddr(
    knSocket fd
)
{
    int opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

// NOTE: Linux makes an IPv6 socket dual-stack by default, but net.ipv6.bindv6only can say otherwise
int knSocket_setDualStack(
    knSocket fd
)
{
    int opt = 0;

    if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

int knSocket_ignorePortUnreachable(
    knSocket fd KN_UNUSED
)
{
    return KNEVTOK;
}

// NOTE: MSG_NOSIGNAL: writing to a peer that is gone gives EPIPE, not a SIGPIPE that kills the process
ssize_t knSocket_send(
    knSocket fd,
    const void *data,
    size_t size
)
{
    return send(fd, data, size, MSG_NOSIGNAL);
}

ssize_t knSocket_recv(
    knSocket fd,
    void *buff,
    size_t size
)
{
    return recv(fd, buff, size, 0);
}

ssize_t knSocket_sendTo(
    knSocket fd,
    const void *data,
    size_t size,
    const knAddr *addr
)
{
    return sendto(fd, data, size, MSG_NOSIGNAL, &addr->any, addr->len);
}

ssize_t knSocket_recvFrom(
    knSocket fd,
    void *buff,
    size_t size,
    knAddr *addr
)
{
    addr->len = sizeof(addr->v6);
    return recvfrom(fd, buff, size, 0, &addr->any, &addr->len);
}

knBool knSocket_wouldBlock(void)
{
    return errno == EAGAIN || errno == EWOULDBLOCK;
}

knBool knSocket_inProgress(void)
{
    return errno == EINPROGRESS;
}

int knSocket_poll(
    knSocket fd,
    uint32_t events,
    int timeoutMs,
    uint32_t *revents
)
{
    struct pollfd p = { .fd = fd, .events = 0, .revents = 0 };
    int status;

    if (events & KN_POLLIN) p.events |= POLLIN;
    if (events & KN_POLLOUT) p.events |= POLLOUT;
    status = poll(&p, 1, timeoutMs);
    *revents = 0;
    if (status <= 0) {
        return status;
    }
    if (p.revents & POLLIN) *revents |= KN_POLLIN;
    if (p.revents & POLLOUT) *revents |= KN_POLLOUT;
    if (p.revents & (POLLERR | POLLNVAL)) *revents |= KN_POLLERR;
    if (p.revents & POLLHUP) *revents |= KN_POLLHUP;
    return status;
}
