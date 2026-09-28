/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Helpers for the network tests (server pump, test clients, callbacks)
*/
#ifndef KRONKNET_TESTS_NET_UTILS_H
    #define KRONKNET_TESTS_NET_UTILS_H
    #include <kronklab/kronklab.h>
    #include "kronknet/callback/callback.h"
    #include "kronknet/connection/connection.h"
    #include "kronknet/macros/errdef.h"
    #include "kronknet/macros/types.h"
    #include "kronknet/server/server.h"
    #include <arpa/inet.h>
    #include <errno.h>
    #include <netinet/in.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>

    #define NET_MAX_CONNS  64    //!< Max connections tracked by the callbacks
    #define NET_TIMEOUT_MS 5000  //!< Max time to wait for something to happen

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   What the callbacks saw, and how they should behave.
 *
 * @note    Each kronklab test runs in its own fork, so this is fresh per test
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct net_state_s {

    int           connects;                //!< onConnect calls
    int           reads;                   //!< onRead calls
    int           writes;                  //!< onWrite calls
    int           disconnects;             //!< onDisconnect calls
    size_t        read_bytes;              //!< Total bytes received
    knConnection *conns[NET_MAX_CONNS];    //!< Connections, in accept order
    int           onConnect_ret;           //!< Returned by onConnect
    knBool        echo;                    //!< Send back what is received
    knBool        disconnect_on_read;      //!< knConnection_disconnect on read

} net_state;

static net_state g_net;

static inline int net_onConnect(knServer *server, knConnection *conn)
{
    (void)server;
    if (g_net.connects < NET_MAX_CONNS) {
        g_net.conns[g_net.connects] = conn;
    }
    g_net.connects++;
    return g_net.onConnect_ret;
}

static inline int net_onRead(knConnection *conn, const void *data, size_t size)
{
    g_net.reads++;
    g_net.read_bytes += size;
    if (g_net.echo) {
        knConnection_send(conn, data, size);
    }
    if (g_net.disconnect_on_read) {
        knConnection_disconnect(conn);
    }
    return 0;
}

static inline int net_onWrite(knConnection *conn)
{
    (void)conn;
    g_net.writes++;
    return 0;
}

static inline int net_onDisconnect(knServer *server, knConnection *conn)
{
    (void)server;
    (void)conn;
    g_net.disconnects++;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Create a server on [port] with the test callbacks (echo enabled)
 */
///////////////////////////////////////////////////////////////////////////////
static inline knServer *net_server(knPort port, knFlags flags)
{
    knServer *server = knServer_create(port, flags);

    AssertNotNull(server, "Server creation on port %d should succeed", port);
    memset(&g_net, 0, sizeof(g_net));
    g_net.onConnect_ret = KNEVTOK;
    g_net.echo = knTrue;
    knServer_setOnConnect(server, &net_onConnect);
    knServer_setOnRead(server, &net_onRead);
    knServer_setOnWrite(server, &net_onWrite);
    knServer_setOnDisconnect(server, &net_onDisconnect);
    return server;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Run the server for [ms] milliseconds
 */
///////////////////////////////////////////////////////////////////////////////
static inline void net_pump(knServer *server, timestamp ms)
{
    timestamp end = kl_monotonic() + ms;

    while (kl_monotonic() < end) {
        AssertEq(knServer_runOnce(server, 5), KNEVTOK, "runOnce should succeed");
    }
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Run the server until [cond] is true, fail after NET_TIMEOUT_MS
 */
///////////////////////////////////////////////////////////////////////////////
    #define NET_PUMP_UNTIL(server, cond) \
    do { \
        timestamp __net_end = kl_monotonic() + NET_TIMEOUT_MS; \
        while (!(cond)) { \
            Assert(kl_monotonic() < __net_end, "Timeout waiting for: " #cond); \
            AssertEq(knServer_runOnce(server, 5), KNEVTOK, "runOnce should succeed"); \
        } \
    } while (0)

static inline void net_setRecvTimeout(int fd, int ms)
{
    struct timeval tv = { .tv_sec = ms / 1000, .tv_usec = (ms % 1000) * 1000 };

    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

static inline struct sockaddr_in net_loopback(knPort port)
{
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return addr;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Connect a TCP client to 127.0.0.1:[port]
 *
 * @param rcvbuf  SO_RCVBUF to set before connecting (0 to keep the default)
 */
///////////////////////////////////////////////////////////////////////////////
static inline int net_tcpClientBuf(knPort port, int rcvbuf)
{
    struct sockaddr_in addr = net_loopback(port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    Assert(fd != -1, "Client socket creation should succeed");
    if (rcvbuf > 0) {
        setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
    }
    net_setRecvTimeout(fd, 1000);
    Assert(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0,
        "Client should connect to port %d", port);
    return fd;
}

static inline int net_tcpClient(knPort port)
{
    return net_tcpClientBuf(port, 0);
}

static inline int net_udpClient(void)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    Assert(fd != -1, "UDP client socket creation should succeed");
    net_setRecvTimeout(fd, 1000);
    return fd;
}

static inline void net_udpSend(int fd, knPort port, const void *data, size_t size)
{
    struct sockaddr_in addr = net_loopback(port);

    AssertEq(sendto(fd, data, size, 0, (struct sockaddr *)&addr, sizeof(addr)),
        (ssize_t)size, "UDP client should send the whole datagram");
}

static inline void net_send(int fd, const void *data, size_t size)
{
    AssertEq(send(fd, data, size, MSG_NOSIGNAL), (ssize_t)size,
        "Client should send the whole message");
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Receive exactly [size] bytes on [fd], running the server meanwhile
 */
///////////////////////////////////////////////////////////////////////////////
static inline void net_recvAll(knServer *server, int fd, void *buf, size_t size)
{
    timestamp end = kl_monotonic() + NET_TIMEOUT_MS;
    size_t got = 0;
    ssize_t r;

    while (got < size) {
        r = recv(fd, (uint8_t *)buf + got, size - got, MSG_DONTWAIT);
        if (r > 0) {
            got += (size_t)r;
            continue;
        }
        Assert(r != 0, "Connection closed after %zu/%zu bytes", got, size);
        Assert(errno == EAGAIN || errno == EWOULDBLOCK, "recv failed (errno %d)", errno);
        Assert(kl_monotonic() < end, "Timeout: received %zu/%zu bytes", got, size);
        AssertEq(knServer_runOnce(server, 0), KNEVTOK, "runOnce should succeed");
    }
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Receive one datagram on [fd], running the server meanwhile
 */
///////////////////////////////////////////////////////////////////////////////
static inline ssize_t net_udpRecv(knServer *server, int fd, void *buf, size_t size)
{
    timestamp end = kl_monotonic() + NET_TIMEOUT_MS;
    ssize_t r;

    while (1) {
        r = recv(fd, buf, size, MSG_DONTWAIT);
        if (r >= 0) {
            return r;
        }
        Assert(errno == EAGAIN || errno == EWOULDBLOCK, "recv failed (errno %d)", errno);
        Assert(kl_monotonic() < end, "Timeout waiting for a datagram");
        AssertEq(knServer_runOnce(server, 5), KNEVTOK, "runOnce should succeed");
    }
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Is [fd] closed by the peer ? (EOF or reset, waits up to 1s)
 */
///////////////////////////////////////////////////////////////////////////////
static inline knBool net_isClosed(int fd)
{
    char c;
    ssize_t r = recv(fd, &c, 1, 0);

    return r == 0 || (r == -1 && errno == ECONNRESET);
}

#endif /* KRONKNET_TESTS_NET_UTILS_H */
