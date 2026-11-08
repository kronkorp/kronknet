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
    int family,
    int type
)
{
    knSocket fd = socket(family, type, 0);

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
    knAddr *addr
)
{
    knSocket fd;

    addr->len = (int)sizeof(addr->v6);
    fd = accept(listener, &addr->any, &addr->len);

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

// NOTE: Windows makes an IPv6 socket IPv6-only by default
int knSocket_setDualStack(
    knSocket fd
)
{
    DWORD opt = 0;

    if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (const char *)&opt, sizeof(opt)) == SOCKET_ERROR) {
        return KNEVTNET;
    }
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

// NOTE: There is no SIGPIPE on Windows, so nothing like MSG_NOSIGNAL is needed.
//       While a non-blocking connect() goes on, POSIX says "try again" (EAGAIN) to send(), so
//       what is sent waits in the out buffer until the socket is writable, i.e. connected.
//       Windows says "not connected" (WSAENOTCONN): say what POSIX says
ssize_t knSocket_send(
    knSocket fd,
    const void *data,
    size_t size
)
{
    int sent = send(fd, (const char *)data, __knSocket_length(size), 0);

    if (sent == SOCKET_ERROR && WSAGetLastError() == WSAENOTCONN) {
        WSASetLastError(WSAEWOULDBLOCK);
    }
    return sent;
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
    const knAddr *addr
)
{
    return sendto(fd, (const char *)data, __knSocket_length(size), 0, &addr->any, addr->len);
}

ssize_t knSocket_recvFrom(
    knSocket fd,
    void *buff,
    size_t size,
    knAddr *addr
)
{
    int length = __knSocket_length(size);

    addr->len = (int)sizeof(addr->v6);
    return __knSocket_received(recvfrom(fd, (char *)buff, length, 0, &addr->any, &addr->len), length);
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

// NOTE: What WSAPoll() called POLLHUP and POLLERR on a stream that is readable: the peer closed it (a read would give 0) or it
//       was reset. (A datagram of 0 bytes is a datagram: UDP says nothing here)
static uint32_t __knSocket_streamEnded(
    knSocket fd
)
{
    int type = 0;
    int length = (int)sizeof(type);
    char byte;
    int peeked;

    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, (char *)&type, &length) == SOCKET_ERROR || type != SOCK_STREAM) {
        return 0;
    }
    peeked = recv(fd, &byte, 1, MSG_PEEK);
    if (peeked == 0) {
        return KN_POLLHUP;
    }
    if (peeked == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        return KN_POLLERR;
    }
    return 0;
}

// NOTE: select(), not WSAPoll(): a non-blocking connect() that fails is reported by select() in the exception set,
//       and WSAPoll() does not tell it on every Windows (before Windows 10 2004 never, and it can still stay silent: the
//       client of KUGE waited for ever on a port where nobody listens). A socket is in one of the three sets, so
//       the status counts it once for each set it is in: only "more than 0" means something
int knSocket_poll(
    knSocket fd,
    uint32_t events,
    int timeoutMs,
    uint32_t *revents
)
{
    fd_set in;
    fd_set out;
    fd_set failed;
    struct timeval wait = { 0, 0 };
    int status;

    FD_ZERO(&in);
    FD_ZERO(&out);
    FD_ZERO(&failed);
    if (events & KN_POLLIN) FD_SET(fd, &in);
    if (events & KN_POLLOUT) FD_SET(fd, &out);
    FD_SET(fd, &failed);
    if (timeoutMs >= 0) {
        wait.tv_sec = timeoutMs / 1000;
        wait.tv_usec = (timeoutMs % 1000) * 1000;
    }
    status = select(0, &in, &out, &failed, timeoutMs >= 0 ? &wait : NULL);
    *revents = 0;
    if (status == SOCKET_ERROR) {
        return -1;
    }
    if (status == 0) {
        return 0;
    }
    if (FD_ISSET(fd, &in)) {
        *revents |= KN_POLLIN;
        *revents |= __knSocket_streamEnded(fd);
    }
    if (FD_ISSET(fd, &out)) *revents |= KN_POLLOUT;
    if (FD_ISSET(fd, &failed)) *revents |= KN_POLLERR;
    return 1;
}
