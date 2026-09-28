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
#include <arpa/inet.h>
#include <kronknet/utils/hashmap/hashmap.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "../pool/pool.h"
#include "../hooks/tcp/tcp.h"
#include "../hooks/udp/udp.h"

static int __knServer_nonBlocking(
    int fd
)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return KNEVTNET;
    }
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

static int __knServer_bind(
    knServer *server,
    knPort port
)
{
    int opt = 1;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) == -1) {
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
    server->pool.epollfd = -1;
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

    server->fd = socket(AF_INET, type, 0);

    if (server->fd == -1) {
        return KNEVTNET;
    }

    if (__knServer_nonBlocking(server->fd) != KNEVTOK) {
        close(server->fd);
        return KNEVTNET;
    }

    if (__knServer_bind(server, port) != KNEVTOK) {
        close(server->fd);
        return KNEVTNET;
    }

    if (type == SOCK_STREAM) {
        if (listen(server->fd, SOMAXCONN) == -1) {
            close(server->fd);
            return KNEVTNET;
        }
    }

    if (knPool_init(&server->pool) != KNEVTOK) {
        close(server->fd);
        return KNEVTERR;
    }

    if (server->flags & knUDP) {
        server->on_udp.connections = knMap_create(knMap_basicHash, 8);
        if (!server->on_udp.connections) {
            knPool_clear(&server->pool);
            close(server->fd);
            return KNEVTMEM;
        }
    }

    // NOTE: The server is registered with a NULL connection (data.ptr)
    if (knPool_registerFd(&server->pool, server->fd, NULL, EPOLLIN) != KNEVTOK) {
        knPool_clear(&server->pool);
        close(server->fd);
        return KNEVTNET;
    }
    return KNEVTOK;
}
