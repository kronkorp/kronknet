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
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.h"

KN_HOT
static int __knServer_processEvents(
    knServer *server,
    const struct epoll_event *events,
    int nfds
)
{
    knConnection *conn;
    int status;

    for (int i = 0; i < nfds; ++i) {
        // NOTE: NULL means the server socket
        conn = events[i].data.ptr;
        if (events[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR) &&
            server->onPollinHook) {
            status = server->onPollinHook(server, conn);
            if (status == KNEVTKICK) {
                continue;
            }
            if (status != KNEVTOK) {
                return KNEVTERR;
            }
        }
        if (events[i].events & EPOLLOUT &&
            server->onPolloutHook &&
            server->onPolloutHook(server, conn) != KNEVTOK) {
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
    struct epoll_event events[KN_MAX_EVENTS];
    int nfds;

    if (!server) {
        return KNEVTARGS;
    }
    nfds = epoll_wait(server->pool.epollfd, events, KN_MAX_EVENTS, (int)timeoutMs);
    if (nfds == -1) {
        return KNEVTNET;
    }
    if (__knServer_processEvents(server, events, nfds) != KNEVTOK) {
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
