/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Run the server. main loop.
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/macros/optimization.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "server.h"

KN_HOT
static int __knServer_processPoll(
    knServer *server
)
{
    for (size_t i = 0; i < server->pool.count; ++i) {
        if (server->pool.pollfds[i].revents & POLLIN &&
            server->onPollinHook &&
            server->onPollinHook(server, &i) != KNEVTOK) {
            return KNEVTERR;
        }
        if (server->pool.pollfds[i].revents & POLLOUT &&
            server->onPolloutHook &&
            server->onPolloutHook(server, &i) != KNEVTOK) {
            return KNEVTERR;
        }
    }
    return KNEVTOK;
}

KN_HOT
KN_API
int knServer_runOnce(
    knServer *server,
    ssize_t timeoutMs
)
{
    int status;

    if (!server) {
        return KNEVTARGS;
    }
    // FIXME: Maybe epoll is better ?
    #ifdef _WIN32
        status = WSAPoll(server->pool.pollfds, (ULONG)server->pool.count, timeoutMs);
    #else
        status = poll(server->pool.pollfds, server->pool.count, timeoutMs);
    #endif /* _WIN32 */
    if (status == -1) {
        return KNEVTNET;
    }
    if (__knServer_processPoll(server) != KNEVTOK) {
        return KNEVTERR;
    }
    if (server->onCleanupHook) {
        server->onCleanupHook(server);
    }
    return KNEVTOK;
}

KN_API
int knServer_run(
    knServer *server
)
{
    if (!server) {
        return KNEVTARGS;
    }
    knInfo(server->logger, "Server (mode=%s) running on %s:%d",
        (server->flags & knTCP) ? "TCP" : "UDP",
        knServer_getIp(server),
        knServer_getPort(server)
    );
    while (server->running) {
        if (knServer_runOnce(server, -1) != KNEVTOK) {
            knFatal(server->logger, "Unexpected fatal error encountered, exiting...");
            break;
        }
    }
    return KNEVTOK;
}
