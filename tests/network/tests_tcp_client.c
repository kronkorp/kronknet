/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Tests of the TCP client
*/
#include <kronklab/kronklab.h>
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include "kronknet/utils/rbuff/rbuff.h"
// NOTE: The client is opaque: the tests look at its out buffer and its socket
#include "../../src/client/client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define CLIENT_PORT 43001
#define CHUNK       1000

//! A server socket that never reads until asked to, with small buffers so that it fills up soon
static int slow_server(uint16_t port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    int small = 4096;
    struct sockaddr_in addr = {0};

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &small, sizeof(small));   // (what it accepts inherits it)
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0 || listen(fd, 4) != 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static uint8_t pattern(size_t index)
{
    return (uint8_t)(index * 31 + 7);
}

static int g_drained = 0;

static int on_drained(knClient *client)
{
    (void)client;
    ++g_drained;
    return KNEVTOK;
}

Test(tcp_client, create_has_an_out_buffer)
{
    knClient *client = knClient_create(knTCP);

    AssertNotNull(client, "TCP client creation should succeed");
    AssertNotNull(client->buff, "A TCP client has an out buffer, for when its socket is full");
    AssertEq(knRBuff_usage(client->buff), 0, "which starts empty");
    knClient_destroy(client);
}

Test(tcp_client, full_socket_is_buffered)
{
    int listener = slow_server(CLIENT_PORT);
    knClient *client = knClient_create(knTCP);
    struct pollfd writable;
    uint8_t chunk[CHUNK];
    size_t accepted = 0;
    int result = KNEVTOK;
    int small = 4096;

    AssertGe(listener, 0, "The test server should listen");
    AssertNotNull(client, "TCP client creation should succeed");
    client->onWrite = on_drained;
    AssertEq(knClient_connect(client, "127.0.0.1", CLIENT_PORT), KNEVTOK, "Connect should succeed");
    writable = (struct pollfd){client->fd, POLLOUT, 0};
    AssertEq(poll(&writable, 1, 2000), 1, "The connection should be made");
    setsockopt(client->fd, SOL_SOCKET, SO_SNDBUF, &small, sizeof(small));

    // Send until the client says no. The socket fills up long before the buffer of the client does.
    for (int i = 0; i < 100000 && result == KNEVTOK; ++i) {
        for (size_t b = 0; b < CHUNK; ++b) {
            chunk[b] = pattern(accepted + b);
        }
        result = knClient_sendServer(client, chunk, CHUNK);
        if (result == KNEVTOK) {
            accepted += CHUNK;
        }
    }
    AssertEq(result, KNEVTKICK, "The client says no only when its buffer is full too");
    Assert(knRBuff_usage(client->buff) > 0, "What did not fit in the socket waited in the buffer of the client");
    AssertGe(accepted, (size_t)CHUNK * 4, "and it took a few messages more than the socket could");

    // The other side reads everything, while the client sends what it kept
    int peer = accept(listener, NULL, NULL);
    size_t received = 0;
    size_t wrong = 0;
    struct timeval start, now;
    uint8_t buffer[4096];

    AssertGe(peer, 0, "The server should accept the client");
    gettimeofday(&start, NULL);
    while (received < accepted) {
        gettimeofday(&now, NULL);
        if ((now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000 > 5000) {
            break;
        }
        knClient_runOnce(client, 5);
        ssize_t got = recv(peer, buffer, sizeof(buffer), MSG_DONTWAIT);
        for (ssize_t i = 0; i < got; ++i) {
            wrong += buffer[i] != pattern(received + (size_t)i);
        }
        received += got > 0 ? (size_t)got : 0;
    }
    AssertEq(received, accepted, "Everything that was accepted should arrive");
    AssertEq(wrong, 0, "in order, and intact");
    Assert(knRBuff_isEmpty(client->buff), "and the buffer of the client is empty again");
    AssertGe(g_drained, 1, "which the client said with onWrite");
    close(peer);
    close(listener);
    knClient_destroy(client);
}

// Once what waited in the buffer of the client is sent, there is nothing to wait for the socket to
// be writable for: runOnce must sleep until something comes in, or its timeout
Test(tcp_client, drained_client_waits)
{
    int listener = slow_server(CLIENT_PORT + 1);
    knClient *client = knClient_create(knTCP);
    struct pollfd writable;
    uint8_t chunk[CHUNK] = {0};
    uint8_t buffer[4096];
    int small = 4096;
    int peer;
    timestamp start;

    AssertGe(listener, 0, "The test server should listen");
    AssertNotNull(client, "TCP client creation should succeed");
    AssertEq(knClient_connect(client, "127.0.0.1", CLIENT_PORT + 1), KNEVTOK, "Connect should succeed");
    writable = (struct pollfd){client->fd, POLLOUT, 0};
    AssertEq(poll(&writable, 1, 2000), 1, "The connection should be made");
    setsockopt(client->fd, SOL_SOCKET, SO_SNDBUF, &small, sizeof(small));

    // Send until the client has to keep something
    for (int i = 0; i < 100000 && knRBuff_isEmpty(client->buff); ++i) {
        AssertEq(knClient_sendServer(client, chunk, CHUNK), KNEVTOK, "The client should take what it is given");
    }
    Assert(!knRBuff_isEmpty(client->buff), "The client should keep what its socket refused");

    // The other side reads everything, until the client has sent all it kept
    peer = accept(listener, NULL, NULL);
    AssertGe(peer, 0, "The server should accept the client");
    start = kl_monotonic();
    while (!knRBuff_isEmpty(client->buff) && kl_monotonic() - start < 5000) {
        knClient_runOnce(client, 5);
        while (recv(peer, buffer, sizeof(buffer), MSG_DONTWAIT) > 0);
    }
    Assert(knRBuff_isEmpty(client->buff), "The client should send all it kept");

    start = kl_monotonic();
    AssertEq(knClient_runOnce(client, 200), KNEVTOK, "An idle runOnce should succeed");
    AssertGe(kl_monotonic() - start, (timestamp)150, "With nothing to send nor to read, runOnce should wait for its timeout");
    close(peer);
    close(listener);
    knClient_destroy(client);
}
