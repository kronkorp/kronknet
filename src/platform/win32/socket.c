/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Sockets on Windows (Winsock).
*/
#include "../socket.h"
#include "kronknet/macros/errdef.h"
#include <limits.h>

// NOTE: Winsock counts who uses it: each WSAStartup needs its WSACleanup
int knSocket_startup(void)
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

void knSocket_cleanup(void)
{
    WSACleanup();
}

static int __knSocket_nonBlocking(
    knSocket fd
)
{
    u_long mode = 1;

    if (ioctlsocket(fd, FIONBIO, &mode) == SOCKET_ERROR) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

knSocket knSocket_open(
    int type
)
{
    knSocket fd = socket(AF_INET, type, 0);

    if (fd == KN_INVALID_SOCKET) {
        return KN_INVALID_SOCKET;
    }
    if (__knSocket_nonBlocking(fd) != KNEVTOK) {
        closesocket(fd);
        return KN_INVALID_SOCKET;
    }
    return fd;
}

knSocket knSocket_accept(
    knSocket listener,
    struct sockaddr_in *addr
)
{
    int len = sizeof(*addr);
    knSocket fd = accept(listener, (struct sockaddr *)addr, &len);

    if (fd == KN_INVALID_SOCKET) {
        return KN_INVALID_SOCKET;
    }
    if (__knSocket_nonBlocking(fd) != KNEVTOK) {
        closesocket(fd);
        return KN_INVALID_SOCKET;
    }
    return fd;
}

// NOTE: A socket is not a file descriptor on Windows: close() would not close it
void knSocket_close(
    knSocket fd
)
{
    if (fd != KN_INVALID_SOCKET) {
        closesocket(fd);
    }
}

int knSocket_setReuseAddr(
    knSocket fd
)
{
    BOOL opt = TRUE;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) == SOCKET_ERROR) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

// NOTE: Winsock counts bytes in an int: a stream sends the rest on the next call,
//       and a datagram can never be that big anyway
static int __knSocket_length(
    size_t size
)
{
    return size > INT_MAX ? INT_MAX : (int)size;
}

// NOTE: There is no SIGPIPE on Windows, so nothing like MSG_NOSIGNAL is needed
ssize_t knSocket_send(
    knSocket fd,
    const void *data,
    size_t size
)
{
    return send(fd, (const char *)data, __knSocket_length(size), 0);
}

ssize_t knSocket_recv(
    knSocket fd,
    void *buff,
    size_t size
)
{
    return recv(fd, (char *)buff, __knSocket_length(size), 0);
}

ssize_t knSocket_sendTo(
    knSocket fd,
    const void *data,
    size_t size,
    const struct sockaddr_in *addr
)
{
    return sendto(fd, (const char *)data, __knSocket_length(size), 0,
        (const struct sockaddr *)addr, (int)sizeof(*addr));
}

ssize_t knSocket_recvFrom(
    knSocket fd,
    void *buff,
    size_t size,
    struct sockaddr_in *addr
)
{
    int len = sizeof(*addr);

    return recvfrom(fd, (char *)buff, __knSocket_length(size), 0, (struct sockaddr *)addr, &len);
}

knBool knSocket_wouldBlock(void)
{
    return WSAGetLastError() == WSAEWOULDBLOCK;
}

// NOTE: A non-blocking connect() says WSAEWOULDBLOCK on Windows, where POSIX says EINPROGRESS
knBool knSocket_inProgress(void)
{
    return WSAGetLastError() == WSAEWOULDBLOCK;
}

// NOTE: WSAPoll() is poll(). Before Windows 10 2004 it did not report a connect() that
//       failed (https://learn.microsoft.com/windows/win32/api/winsock2/nf-winsock2-wsapoll)
int knSocket_poll(
    knSocket fd,
    uint32_t events,
    int timeoutMs,
    uint32_t *revents
)
{
    WSAPOLLFD p = { .fd = fd, .events = 0, .revents = 0 };
    int status;

    if (events & KN_POLLIN) p.events |= POLLIN;
    if (events & KN_POLLOUT) p.events |= POLLOUT;
    status = WSAPoll(&p, 1, timeoutMs);
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
