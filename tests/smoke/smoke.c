/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Smoke test: a client and a server talk, in TCP then in UDP. Only the public
** API, no kronklab (it needs fork()): it runs on every platform.
*/
#include "kronknet/client/client.h"
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/server/server.h"
#include "kronknet/utils/monotonic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMOKE_TIMEOUT_MS 5000
#define SMOKE_MESSAGE    "kronk"

static char   g_received[64];
static size_t g_receivedSize;

static int smoke_echo(knConnection *conn, const void *data, size_t size)
{
    return knConnection_send(conn, data, size);
}

static int smoke_collect(knClient *client, const void *data, size_t size)
{
    (void)client;
    if (size > sizeof(g_received) - g_receivedSize) {
        size = sizeof(g_received) - g_receivedSize;
    }
    memcpy(g_received + g_receivedSize, data, size);
    g_receivedSize += size;
    return KNEVTOK;
}

// The client sends right after knClient_connect, while the connection may
// still be being made: what it sends must wait for it, not be lost
static int smoke_run(knServer *server, knClient *client, knPort port)
{
    timestamp start;

    if (knClient_connect(client, "127.0.0.1", port) != KNEVTOK) {
        return 0;
    }
    if (knClient_sendServer(client, SMOKE_MESSAGE, strlen(SMOKE_MESSAGE)) != KNEVTOK) {
        return 0;
    }
    start = monotonic();
    while (g_receivedSize < strlen(SMOKE_MESSAGE) && monotonic() - start < SMOKE_TIMEOUT_MS) {
        if (knServer_runOnce(server, 10) != KNEVTOK || knClient_runOnce(client, 10) != KNEVTOK) {
            return 0;
        }
    }
    return g_receivedSize == strlen(SMOKE_MESSAGE)
        && memcmp(g_received, SMOKE_MESSAGE, g_receivedSize) == 0;
}

static int smoke(const char *name, knFlags flags, knPort port)
{
    knServer *server = knServer_create(port, flags);
    knClient *client = knClient_create(flags);
    int ok = 0;

    g_receivedSize = 0;
    if (server && client) {
        knServer_setOnRead(server, &smoke_echo);
        knClient_setOnRead(client, &smoke_collect);
        ok = smoke_run(server, client, port);
    }
    printf("%s echo on port %d: %s\n", name, port, ok ? "ok" : "FAILED");
    knClient_destroy(client);
    knServer_destroy(server);
    return ok;
}

int main(int argc, char **argv)
{
    knPort port = (argc > 1) ? (knPort)atoi(argv[1]) : 42900;
    int ok = 1;

    ok &= smoke("TCP", knTCP, port);
    ok &= smoke("UDP", knUDP, (knPort)(port + 1));
    return ok ? 0 : 1;
}
