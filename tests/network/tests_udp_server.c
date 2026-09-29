#include "net_utils.h"
// NOTE: The server is opaque: this test looks at its epoll
#include "../../src/server/server.h"
#include <stdio.h>
#include <sys/epoll.h>

Test(udp_server, create_destroy)
{
    knServer *server = knServer_create(42201, knUDP);

    AssertNotNull(server, "UDP server creation should succeed");
    Assert(knServer_isRunning(server), "Server should be running after creation");
    AssertEq(knServer_getPort(server), 42201, "Port should be the one given at creation");
    knServer_destroy(server);
}

Test(udp_server, idle_runonce_waits_timeout)
{
    knServer *server = net_server(42202, knUDP);
    timestamp start = kl_monotonic();

    AssertEq(knServer_runOnce(server, 100), KNEVTOK, "Idle runOnce should succeed");
    AssertGe(kl_monotonic() - start, (timestamp)90, "Idle runOnce should block until its timeout");
    knServer_destroy(server);
}

Test(udp_server, echo)
{
    knServer *server = net_server(42203, knUDP);
    int fd = net_udpClient();
    char buf[64] = {0};

    net_udpSend(fd, 42203, "ping", 4);
    AssertEq(net_udpRecv(server, fd, buf, sizeof(buf)), (ssize_t)4, "Echo should be 4 bytes");
    AssertEq(memcmp(buf, "ping", 4), 0, "Echoed datagram should match");
    AssertEq(g_net.connects, 1, "First datagram should create a connection");
    AssertEq(g_net.reads, 1, "onRead should be called once");
    close(fd);
    knServer_destroy(server);
}

Test(udp_server, big_datagram_echo)
{
    knServer *server = net_server(42204, knUDP);
    int fd = net_udpClient();
    uint8_t msg[4000];
    uint8_t buf[sizeof(msg)];

    for (size_t i = 0; i < sizeof(msg); ++i) {
        msg[i] = (uint8_t)(i * 7);
    }
    net_udpSend(fd, 42204, msg, sizeof(msg));
    AssertEq(net_udpRecv(server, fd, buf, sizeof(buf)), (ssize_t)sizeof(msg), "Whole datagram should be echoed");
    AssertEq(memcmp(buf, msg, sizeof(msg)), 0, "Echoed datagram should match");
    close(fd);
    knServer_destroy(server);
}

Test(udp_server, same_client_same_connection)
{
    knServer *server = net_server(42205, knUDP);
    int fd = net_udpClient();
    char buf[16];

    for (int i = 0; i < 3; ++i) {
        net_udpSend(fd, 42205, "again", 5);
        AssertEq(net_udpRecv(server, fd, buf, sizeof(buf)), (ssize_t)5, "Echo should be 5 bytes");
    }
    AssertEq(g_net.reads, 3, "onRead should be called for each datagram");
    AssertEq(g_net.connects, 1, "A single client should be a single connection");
    close(fd);
    knServer_destroy(server);
}

Test(udp_server, many_clients_get_their_own_echo)
{
    knServer *server = net_server(42206, knUDP);
    int fds[10];
    char msg[16];
    char buf[16];

    for (int i = 0; i < 10; ++i) {
        fds[i] = net_udpClient();
        snprintf(msg, sizeof(msg), "udp-%02d", i);
        net_udpSend(fds[i], 42206, msg, 6);
    }
    for (int i = 0; i < 10; ++i) {
        snprintf(msg, sizeof(msg), "udp-%02d", i);
        AssertEq(net_udpRecv(server, fds[i], buf, sizeof(buf)), (ssize_t)6, "Echo should be 6 bytes");
        AssertEq(memcmp(buf, msg, 6), 0, "Client %d should receive its own echo", i);
    }
    AssertEq(g_net.connects, 10, "Each client should have its own connection");
    for (int i = 0; i < 10; ++i) {
        close(fds[i]);
    }
    knServer_destroy(server);
}

Test(udp_server, timeout_removes_clients)
{
    knServer *server = net_server(42207, knUDP);
    timestamp start = kl_monotonic();
    int fds[3];
    char buf[16];

    knServer_setConnectionTimeout(server, 200);
    for (int i = 0; i < 3; ++i) {
        fds[i] = net_udpClient();
        net_udpSend(fds[i], 42207, "hey", 3);
        AssertEq(net_udpRecv(server, fds[i], buf, sizeof(buf)), (ssize_t)3, "Echo should be 3 bytes");
    }
    NET_PUMP_UNTIL(server, g_net.disconnects == 3);
    AssertGe(kl_monotonic() - start, (timestamp)200, "Clients should not be removed before the timeout");

    // NOTE: A removed client talking again is a new connection
    net_udpSend(fds[0], 42207, "back", 4);
    AssertEq(net_udpRecv(server, fds[0], buf, sizeof(buf)), (ssize_t)4, "Echo should be 4 bytes");
    AssertEq(g_net.connects, 4, "A timed out client should get a new connection");
    for (int i = 0; i < 3; ++i) {
        close(fds[i]);
    }
    knServer_destroy(server);
}

Test(udp_server, active_client_is_not_removed)
{
    knServer *server = net_server(42208, knUDP);
    int fd = net_udpClient();
    char buf[16];

    knServer_setConnectionTimeout(server, 300);
    for (int i = 0; i < 10; ++i) {
        net_udpSend(fd, 42208, "ping", 4);
        AssertEq(net_udpRecv(server, fd, buf, sizeof(buf)), (ssize_t)4, "Echo should be 4 bytes");
        net_pump(server, 100);
    }
    AssertEq(g_net.disconnects, 0, "An active client should never be removed");
    AssertEq(g_net.connects, 1, "An active client should keep its connection");
    close(fd);
    knServer_destroy(server);
}

// A datagram is sent whole, now, and nothing is kept: there is nothing to wait for the socket to be
// writable for, so sending must not ask epoll to say so (the socket is nearly always writable: each
// datagram would wake the server up for nothing, and take the pollout hook through all its connections)
Test(udp_server, send_does_not_arm_epollout)
{
    knServer *server = net_server(42209, knUDP);
    int fd = net_udpClient();
    struct epoll_event events[4];
    char buf[16];

    g_net.echo = knFalse;   // this test sends by hand
    net_udpSend(fd, 42209, "hi", 2);
    NET_PUMP_UNTIL(server, g_net.reads == 1);
    AssertEq(epoll_wait(server->pool.epollfd, events, 4, 0), 0, "Nothing to report once the datagram was read");
    AssertEq(knConnection_send(g_net.conns[0], "pong", 4), KNEVTOK, "The server should send a datagram");
    AssertEq(epoll_wait(server->pool.epollfd, events, 4, 0), 0, "Sending should not make epoll wake the server up");
    AssertEq(recv(fd, buf, sizeof(buf), 0), (ssize_t)4, "The datagram should arrive");
    AssertEq(memcmp(buf, "pong", 4), 0, "and be the one that was sent");
    close(fd);
    knServer_destroy(server);
}
