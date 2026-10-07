/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Sockets on Windows (Winsock).
*/
#include "../socket.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/optimization.h"
#include <limits.h>

// NOTE: In <mstcpip.h> with MSVC, in <mswsock.h> with MinGW
#ifndef SIO_UDP_CONNRESET
    #define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

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

// NOTE: Windows already lets a port be bound again while its old connections are in TIME_WAIT.
//       Its SO_REUSEADDR is something else: it lets a socket bind a port another socket is
//       listening on, and take its connections
int knSocket_setReuseAddr(
    knSocket fd KN_UNUSED
)
{
    return KNEVTOK;
}

int knSocket_ignorePortUnreachable(
    knSocket fd
)
{
    BOOL report = FALSE;
    DWORD bytes = 0;

    if (WSAIoctl(fd, SIO_UDP_CONNRESET, &report, sizeof(report),
        NULL, 0, &bytes, NULL, NULL) == SOCKET_ERROR) {
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

// NOTE: A datagram bigger than the buffer: POSIX gives what fits, Windows gives the same
//       but calls it an error (WSAEMSGSIZE). Say what POSIX says
static ssize_t __knSocket_received(
    int received,
    int length
)
{
    if (received == SOCKET_ERROR && WSAGetLastError() == WSAEMSGSIZE) {
        return length;
    }
    return received;
}

ssize_t knSocket_recv(
    knSocket fd,
    void *buff,
    size_t size
)
{
    int length = __knSocket_length(size);

    return __knSocket_received(recv(fd, (char *)buff, length, 0), length);
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
    int length = __knSocket_length(size);
    int len = sizeof(*addr);

    return __knSocket_received(recvfrom(fd, (char *)buff, length, 0, (struct sockaddr *)addr, &len), length);
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
