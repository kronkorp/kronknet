#include "net_utils.h"
// NOTE: The server is opaque: this test looks at its poller, and at its connections
#include "../../src/server/server.h"
#include <stdio.h>
#include "../../src/connection/connection.h"
#include "../../src/platform/poller.h"
#include "../../src/server/hooks/udp/udp.h"
#include "../../src/utils/address/address.h"

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
    knPollEvent events[4];
    char buf[16];

    g_net.echo = knFalse;   // this test sends by hand
    net_udpSend(fd, 42209, "hi", 2);
    NET_PUMP_UNTIL(server, g_net.reads == 1);
    AssertEq(knPoller_wait(server->pool.poller, events, 4, 0), 0, "Nothing to report once the datagram was read");
    AssertEq(knConnection_send(g_net.conns[0], "pong", 4), KNEVTOK, "The server should send a datagram");
    AssertEq(knPoller_wait(server->pool.poller, events, 4, 0), 0, "Sending should not make the poller wake the server up");
    AssertEq(recv(fd, buf, sizeof(buf), 0), (ssize_t)4, "The datagram should arrive");
    AssertEq(memcmp(buf, "pong", 4), 0, "and be the one that was sent");
    close(fd);
    knServer_destroy(server);
}

Test(udp_server, ipv6_echo)
{
    knServer *server = net_server(42210, knUDP);
    int fd = net_udpClient6();
    char buf[16] = {0};

    net_udpSend6(fd, 42210, "ping6", 5);
    AssertEq(net_udpRecv(server, fd, buf, sizeof(buf)), (ssize_t)5, "Echo should be 5 bytes");
    AssertEq(memcmp(buf, "ping6", 5), 0, "Echoed datagram should match");
    AssertEq(g_net.connects, 1, "First datagram should create a connection");
    AssertStrEq(knConnection_getIp(g_net.conns[0]), "::1", "Connection ip should be the IPv6 loopback");
    close(fd);
    knServer_destroy(server);
}

Test(udp_server, ipv4_and_ipv6_peers)
{
    knServer *server = net_server(42211, knUDP);
    int fd4 = net_udpClient();
    int fd6 = net_udpClient6();
    char buf[16];

    net_udpSend(fd4, 42211, "four", 4);
    AssertEq(net_udpRecv(server, fd4, buf, sizeof(buf)), (ssize_t)4, "The IPv4 peer should get its echo");
    AssertEq(memcmp(buf, "four", 4), 0, "Echoed datagram should match");
    net_udpSend6(fd6, 42211, "six", 3);
    AssertEq(net_udpRecv(server, fd6, buf, sizeof(buf)), (ssize_t)3, "The IPv6 peer should get its echo");
    AssertEq(memcmp(buf, "six", 3), 0, "Echoed datagram should match");
    AssertEq(g_net.connects, 2, "Each peer should have its own connection");
    AssertStrEq(knConnection_getIp(g_net.conns[0]), "127.0.0.1", "An IPv4 peer is shown as IPv4, not as ::ffff:127.0.0.1");
    AssertStrEq(knConnection_getIp(g_net.conns[1]), "::1", "An IPv6 peer is shown as IPv6");
    close(fd4);
    close(fd6);
    knServer_destroy(server);
}

//! An IPv6 peer of 2001:db8::/64, made by hand: the loopback has a single IPv6
static knAddr same_key_peer(uint64_t iid, uint16_t port)
{
    static const uint8_t prefix[8] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0};
    knAddr addr = {0};

    addr.v6.sin6_family = AF_INET6;
    addr.v6.sin6_port = port;
    memcpy(addr.v6.sin6_addr.s6_addr, prefix, sizeof(prefix));
    memcpy(addr.v6.sin6_addr.s6_addr + 8, &iid, sizeof(iid));
    addr.len = sizeof(addr.v6);
    return addr;
}

//! Three peers of the same /64 whose addresses make up for their ports: knAddr_hash gives them the same key
static void same_key_peers(knAddr addrs[3])
{
    addrs[0] = same_key_peer(0x1234, 0x1111);
    addrs[1] = same_key_peer(0x1234 ^ 0x1111 ^ 0x2222, 0x2222);
    addrs[2] = same_key_peer(0x1234 ^ 0x1111 ^ 0x3333, 0x3333);
    for (int i = 1; i < 3; ++i) {
        AssertEq(knAddr_hash(&addrs[i]), knAddr_hash(&addrs[0]), "Peer %d should have the key of peer 0 (what this test is built on)", i);
        Assert(!knAddr_equal(&addrs[i], &addrs[0]), "Peer %d should not be peer 0", i);
    }
}

// Two IPv6 peers can get the same key in the map of the connections: each must keep its own
Test(udp_server, same_key_peers)
{
    knServer *server = knServer_create(42212, knUDP);
    knAddr addrs[3];
    knConnection *conns[3];

    AssertNotNull(server, "UDP server creation should succeed");
    same_key_peers(addrs);
    for (int i = 0; i < 3; ++i) {
        conns[i] = knConnection_create(&addrs[i], knUDP);
        AssertNotNull(conns[i], "Connection creation should succeed");
        AssertEq(knServer_udpAdd(server, conns[i]), KNEVTOK, "Connection %d should be added", i);
    }
    for (int i = 0; i < 3; ++i) {
        AssertEq(knServer_udpFind(server, &addrs[i]), conns[i], "Peer %d should find its own connection", i);
    }

    knServer_udpRemove(server, conns[1]);
    knConnection_destroy(conns[1]);
    AssertNull(knServer_udpFind(server, &addrs[1]), "A removed peer should not be found");
    AssertEq(knServer_udpFind(server, &addrs[0]), conns[0], "Removing a connection after the first keeps the first");
    AssertEq(knServer_udpFind(server, &addrs[2]), conns[2], "and the one after it");

    knServer_udpRemove(server, conns[0]);
    knConnection_destroy(conns[0]);
    AssertNull(knServer_udpFind(server, &addrs[0]), "A removed peer should not be found");
    AssertEq(knServer_udpFind(server, &addrs[2]), conns[2], "Removing the first connection of a key keeps the next one");

    // NOTE: conns[2] is still there: the server destroys it
    knServer_destroy(server);
}

Test(udp_server, same_key_timeout)
{
    knServer *server = net_server(42213, knUDP);
    knAddr addrs[3];
    knConnection *conns[3];

    same_key_peers(addrs);
    knServer_setConnectionTimeout(server, 1000);
    for (int i = 0; i < 3; ++i) {
        conns[i] = knConnection_create(&addrs[i], knUDP);
        AssertNotNull(conns[i], "Connection creation should succeed");
        AssertEq(knServer_udpAdd(server, conns[i]), KNEVTOK, "Connection %d should be added", i);
    }
    // NOTE: The first and the last connections of the key have been idle since the clock started
    conns[0]->last_data = 0;
    conns[2]->last_data = 0;
    AssertEq(knServer_runOnce(server, 0), KNEVTOK, "runOnce should succeed");
    AssertEq(g_net.disconnects, 2, "Both idle peers should be removed");
    AssertNull(knServer_udpFind(server, &addrs[0]), "The first idle peer should be gone");
    AssertEq(knServer_udpFind(server, &addrs[1]), conns[1], "The active peer should stay");
    AssertNull(knServer_udpFind(server, &addrs[2]), "The last idle peer should be gone");
    knServer_destroy(server);
}
