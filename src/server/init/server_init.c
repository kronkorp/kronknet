/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "../pool/pool.h"
#include "../server.h"
#include "kronknet/macros/types.h"
#include <kronknet/utils/hashmap/hashmap.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../platform/socket.h"
#include "../hooks/tcp/tcp.h"
#include "../hooks/udp/udp.h"

// NOTE: Undo what knServer_init did so far: knServer_clear must not close the socket again
static int __knServer_abort(
    knServer *server,
    int err
)
{
    knPool_clear(&server->pool);
    knSocket_close(server->fd);
    server->fd = KN_INVALID_SOCKET;
    return err;
}

static int __knServer_bind(
    knServer *server,
    knPort port
)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (knSocket_setReuseAddr(server->fd) != KNEVTOK) {
        return KNEVTNET;
    }
    if (bind(server->fd, (const struct sockaddr *)&server->addr,
        sizeof(server->addr)) == -1) {
        return KNEVTNET;
    }
    if (getsockname(server->fd, (struct sockaddr *)&addr, &len) == -1) {
        return KNEVTNET;
    }
    if (inet_ntop(AF_INET, &addr.sin_addr, server->ip,
        INET_ADDRSTRLEN) == NULL) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

static void __knServer_basics(
    knServer *server,
    knFlags flags
)
{
    server->flags = flags;
    server->running = true;
    server->fd = KN_INVALID_SOCKET;
    server->pool.poller = NULL;
    server->onConnection = NULL;
    server->onWrite = NULL;
    server->onRead = NULL;
    server->onDisconnect = NULL;
    server->logger = (knLoggerData){
        .out = NULL,
        .log_level = knLogNone,
    };
}

static void __knServer_initHooks(
    knServer *server
)
{
    if (server->flags & knTCP) {
        server->connection_timeout = 180000;
        server->onPollinHook  = &knServer_tcpPollinHook;
        server->onPolloutHook = &knServer_tcpPolloutHook;
        server->onCleanupHook = &knServer_tcpCleanupHook;
        server->onDestroyHook = NULL;
    } else if (server->flags & knUDP) {
        server->connection_timeout = 30000;
        server->onPollinHook  = &knServer_udpPollinHook;
        server->onPolloutHook = &knServer_udpPolloutHook;
        server->onCleanupHook = &knServer_udpCleanupHook;
        server->onDestroyHook = &knServer_udpDestroyHook;
    }
}

int knServer_init(
    knServer *server,
    knPort port,
    knFlags flags
)
{
    // NOTE: By default, protocol is TCP
    int type = SOCK_STREAM;

    if (!server)
        return KNEVTERR;

    __knServer_basics(server, flags);
    __knServer_initHooks(server);

    if (flags & knTCP && flags & knUDP) {
        return KNEVTARGS;
    }
    if (flags & knTCP) type = SOCK_STREAM;
    else if (flags & knUDP) type = SOCK_DGRAM;

    server->fd = knSocket_open(type);

    if (server->fd == KN_INVALID_SOCKET) {
        return KNEVTNET;
    }

    if (__knServer_bind(server, port) != KNEVTOK) {
        return __knServer_abort(server, KNEVTNET);
    }

    if (type == SOCK_STREAM) {
        if (listen(server->fd, SOMAXCONN) == -1) {
            return __knServer_abort(server, KNEVTNET);
        }
    } else if (knSocket_ignorePortUnreachable(server->fd) != KNEVTOK) {
        return __knServer_abort(server, KNEVTNET);
    }

    if (knPool_init(&server->pool) != KNEVTOK) {
        return __knServer_abort(server, KNEVTERR);
    }

    if (server->flags & knUDP) {
        server->on_udp.connections = knMap_create(knMap_basicHash, 8);
        if (!server->on_udp.connections) {
            return __knServer_abort(server, KNEVTMEM);
        }
    }

    // NOTE: The server is registered with a NULL connection (data.ptr)
    if (knPool_registerFd(&server->pool, server->fd, NULL, KN_POLLIN) != KNEVTOK) {
        return __knServer_abort(server, KNEVTNET);
    }
    return KNEVTOK;
}
