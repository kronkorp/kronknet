#include "net_utils.h"
#include <stdio.h>
#include <stdlib.h>

Test(tcp_server, create_destroy)
{
    knServer *server = knServer_create(42101, knTCP);

    AssertNotNull(server, "TCP server creation should succeed");
    Assert(knServer_isRunning(server), "Server should be running after creation");
    AssertEq(knServer_getPort(server), 42101, "Port should be the one given at creation");
    knServer_stop(server);
    Assert(!knServer_isRunning(server), "Server should not be running after stop");
    knServer_destroy(server);
}

Test(tcp_server, invalid_flags)
{
    AssertNull(knServer_create(42102, knTCP | knUDP), "TCP and UDP together should fail");
}

Test(tcp_server, port_already_used)
{
    knServer *server = knServer_create(42103, knTCP);

    AssertNotNull(server, "First server should be created");
    AssertNull(knServer_create(42103, knTCP), "Second server on the same port should fail");
    knServer_destroy(server);
}

Test(tcp_server, runonce_null_server)
{
    AssertEq(knServer_runOnce(NULL, 0), KNEVTARGS, "runOnce(NULL) should return KNEVTARGS");
}

Test(tcp_server, idle_runonce_waits_timeout)
{
    knServer *server = net_server(42104, knTCP);
    timestamp start = kl_monotonic();

    AssertEq(knServer_runOnce(server, 100), KNEVTOK, "Idle runOnce should succeed");
    AssertGe(kl_monotonic() - start, (timestamp)90, "Idle runOnce should block until its timeout");
    AssertEq(g_net.connects, 0, "No connection should be reported");
    knServer_destroy(server);
}

Test(tcp_server, accept_calls_onConnect)
{
    knServer *server = net_server(42105, knTCP);
    int fd = net_tcpClient(42105);
    struct sockaddr_in local = {0};
    socklen_t len = sizeof(local);

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    AssertNotNull(g_net.conns[0], "onConnect should receive the connection");
    getsockname(fd, (struct sockaddr *)&local, &len);
    AssertEq(knConnection_getPort(g_net.conns[0]), ntohs(local.sin_port),
        "Connection port should be the client's port");
    AssertStrEq(knConnection_getIp(g_net.conns[0]), "127.0.0.1", "Connection ip should be loopback");
    net_pump(server, 50);
    AssertEq(g_net.connects, 1, "onConnect should be called only once");
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, echo)
{
    knServer *server = net_server(42106, knTCP);
    int fd = net_tcpClient(42106);
    const char msg[] = "hello kronknet";
    char buf[sizeof(msg)] = {0};

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    net_send(fd, msg, sizeof(msg));
    net_recvAll(server, fd, buf, sizeof(msg));
    AssertEq(memcmp(buf, msg, sizeof(msg)), 0, "Echoed data should match");
    AssertEq(g_net.read_bytes, sizeof(msg), "onRead should have received every byte");
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, many_clients_get_their_own_echo)
{
    knServer *server = net_server(42107, knTCP);
    int fds[20];
    char msg[16];
    char buf[16];

    for (int i = 0; i < 20; ++i) {
        fds[i] = net_tcpClient(42107);
    }
    NET_PUMP_UNTIL(server, g_net.connects == 20);
    for (int i = 0; i < 20; ++i) {
        snprintf(msg, sizeof(msg), "client-%02d", i);
        net_send(fds[i], msg, 9);
    }
    for (int i = 0; i < 20; ++i) {
        snprintf(msg, sizeof(msg), "client-%02d", i);
        net_recvAll(server, fds[i], buf, 9);
        AssertEq(memcmp(buf, msg, 9), 0, "Client %d should receive its own echo", i);
    }
    for (int i = 0; i < 20; ++i) {
        close(fds[i]);
    }
    NET_PUMP_UNTIL(server, g_net.disconnects == 20);
    knServer_destroy(server);
}

Test(tcp_server, disconnect_calls_onDisconnect)
{
    knServer *server = net_server(42108, knTCP);
    int fd = net_tcpClient(42108);

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    close(fd);
    NET_PUMP_UNTIL(server, g_net.disconnects == 1);
    net_pump(server, 50);
    AssertEq(g_net.disconnects, 1, "onDisconnect should be called only once");
    knServer_destroy(server);
}

Test(tcp_server, disconnect_keeps_others_alive)
{
    knServer *server = net_server(42109, knTCP);
    int fds[6];
    char msg[16];
    char buf[16];

    for (int i = 0; i < 6; ++i) {
        fds[i] = net_tcpClient(42109);
    }
    NET_PUMP_UNTIL(server, g_net.connects == 6);
    close(fds[1]);
    close(fds[3]);
    NET_PUMP_UNTIL(server, g_net.disconnects == 2);
    for (int i = 0; i < 6; ++i) {
        if (i == 1 || i == 3)
            continue;
        snprintf(msg, sizeof(msg), "still-%02d", i);
        net_send(fds[i], msg, 8);
        net_recvAll(server, fds[i], buf, 8);
        AssertEq(memcmp(buf, msg, 8), 0, "Client %d should still get its echo", i);
    }
    fds[1] = net_tcpClient(42109);
    NET_PUMP_UNTIL(server, g_net.connects == 7);
    net_send(fds[1], "newcomer", 8);
    net_recvAll(server, fds[1], buf, 8);
    AssertEq(memcmp(buf, "newcomer", 8), 0, "A new client should work after disconnections");
    for (int i = 0; i < 6; ++i) {
        if (i != 3)
            close(fds[i]);
    }
    knServer_destroy(server);
}

Test(tcp_server, connection_reset_is_kicked)
{
    knServer *server = net_server(42110, knTCP);
    int fd = net_tcpClient(42110);
    struct linger lg = { .l_onoff = 1, .l_linger = 0 };

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    // NOTE: linger 0 makes close() send a RST (EPOLLHUP / EPOLLERR)
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
    close(fd);
    NET_PUMP_UNTIL(server, g_net.disconnects == 1);
    knServer_destroy(server);
}

Test(tcp_server, onConnect_error_kicks_client)
{
    knServer *server = net_server(42111, knTCP);
    int fd;

    g_net.onConnect_ret = KNEVTKICK;
    fd = net_tcpClient(42111);
    NET_PUMP_UNTIL(server, g_net.disconnects == 1);
    AssertEq(g_net.connects, 1, "onConnect should have been called");
    Assert(net_isClosed(fd), "Client should be closed by the server");
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, inactivity_timeout_kicks_client)
{
    knServer *server = net_server(42112, knTCP);
    timestamp start = kl_monotonic();
    int fd;

    knServer_setConnectionTimeout(server, 200);
    fd = net_tcpClient(42112);
    NET_PUMP_UNTIL(server, g_net.connects == 1);
    NET_PUMP_UNTIL(server, g_net.disconnects == 1);
    AssertGe(kl_monotonic() - start, (timestamp)200, "Client should not be kicked before the timeout");
    Assert(net_isClosed(fd), "Client should be closed by the server");
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, active_client_is_not_kicked)
{
    knServer *server = net_server(42113, knTCP);
    char buf[4];
    int fd;

    knServer_setConnectionTimeout(server, 300);
    fd = net_tcpClient(42113);
    NET_PUMP_UNTIL(server, g_net.connects == 1);
    for (int i = 0; i < 10; ++i) {
        net_send(fd, "ping", 4);
        net_recvAll(server, fd, buf, 4);
        net_pump(server, 100);
    }
    AssertEq(g_net.disconnects, 0, "An active client should never be kicked");
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, disconnect_api_kicks_client)
{
    knServer *server = net_server(42114, knTCP);
    int fd = net_tcpClient(42114);

    g_net.echo = knFalse;
    g_net.disconnect_on_read = knTrue;
    NET_PUMP_UNTIL(server, g_net.connects == 1);
    net_send(fd, "bye", 3);
    NET_PUMP_UNTIL(server, g_net.disconnects == 1);
    Assert(net_isClosed(fd), "Client should be closed by the server");
    close(fd);
    knServer_destroy(server);
}

static uint8_t pattern(size_t offset)
{
    return (uint8_t)(offset % 251);
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Send to [conn] until its out buffer is full (the client [fd] does
 *          not read), then check that everything arrives in order and that
 *          the server goes back to idle.
 */
///////////////////////////////////////////////////////////////////////////////
static void flood_and_check(knServer *server, knConnection *conn, int fd)
{
    uint8_t chunk[1024];
    uint8_t *received;
    size_t sent = 0;
    int ret = KNEVTOK;
    timestamp start;

    // NOTE: The client doesn't read, so the socket fills up, then the out buffer
    while (sent < 64UL * 1024 * 1024) {
        for (size_t i = 0; i < sizeof(chunk); ++i) {
            chunk[i] = pattern(sent + i);
        }
        ret = knConnection_send(conn, chunk, sizeof(chunk));
        if (ret != KNEVTOK)
            break;
        sent += sizeof(chunk);
    }
    AssertEq(ret, KNEVTKICK, "Sending should eventually report a full out buffer");
    AssertEq(g_net.writes, 0, "onWrite should not be called before pollout");

    received = malloc(sent);
    AssertNotNull(received, "malloc should succeed");
    net_recvAll(server, fd, received, sent);
    for (size_t i = 0; i < sent; ++i) {
        AssertEq(received[i], pattern(i), "Byte %zu should arrive in order", i);
    }
    free(received);
    NET_PUMP_UNTIL(server, g_net.writes >= 1);

    // NOTE: Once flushed, EPOLLOUT must be removed, or epoll_wait would not block
    start = kl_monotonic();
    AssertEq(knServer_runOnce(server, 150), KNEVTOK, "runOnce should succeed");
    AssertGe(kl_monotonic() - start, (timestamp)100, "Server should be idle once the data is flushed");
    AssertEq(g_net.disconnects, 0, "Client should not have been kicked");
}

Test(tcp_server, pollout_flushes_in_order)
{
    knServer *server = net_server(42115, knTCP);
    int fd = net_tcpClientBuf(42115, 4096);

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    flood_and_check(server, g_net.conns[0], fd);
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, pollout_after_pool_growth)
{
    knServer *server = net_server(42117, knTCP);
    int fd = net_tcpClientBuf(42117, 4096);
    int others[10];

    NET_PUMP_UNTIL(server, g_net.connects == 1);
    // NOTE: More clients make the pool grow after the first one is registered
    for (int i = 0; i < 10; ++i) {
        others[i] = net_tcpClient(42117);
    }
    NET_PUMP_UNTIL(server, g_net.connects == 11);
    flood_and_check(server, g_net.conns[0], fd);
    for (int i = 0; i < 10; ++i) {
        close(others[i]);
    }
    close(fd);
    knServer_destroy(server);
}

Test(tcp_server, destroy_closes_clients)
{
    knServer *server = net_server(42116, knTCP);
    int fds[5];

    for (int i = 0; i < 5; ++i) {
        fds[i] = net_tcpClient(42116);
    }
    NET_PUMP_UNTIL(server, g_net.connects == 5);
    knServer_destroy(server);
    for (int i = 0; i < 5; ++i) {
        Assert(net_isClosed(fds[i]), "Client %d should be closed when the server is destroyed", i);
        close(fds[i]);
    }
}
