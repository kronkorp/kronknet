/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the server. main loop.
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/macros/optimization.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.h"
#include "../connection/connection.h"

static int __knServer_onPollout(
    knServer *server,
    size_t fdIdx
)
{
    uint8_t tmp[KNBUFFSIZ] = {};
    knConnection *conn = server->pool.conns[fdIdx];
    size_t usage = knRBuff_usage(conn->out_buff);

    knInfo(server->logger, "Connection [%d]: Attempting to send some data", conn->fd);
    knRBuff_peek(conn->out_buff, tmp, usage);
    ssize_t sends = send(conn->fd, tmp, usage, MSG_NOSIGNAL);
    if (sends > 0) {
        knRBuff_pop(conn->out_buff, NULL, sends);
        knInfo(server->logger, "Connection [%d]: sent %zu bytes, remaining: %zu bytes.", conn->fd, (size_t)sends, knRBuff_usage(conn->out_buff));
        if (knRBuff_isEmpty(conn->out_buff)) {
            knConnection_setEvents(server->pool.conns[fdIdx], POLLIN);
            if (server->onWrite) {
                server->onWrite(conn);
            }
        }
    } else {
        knError(server->logger, "Connection [%d]: Failed to send data, remaining: %zu bytes.", conn->fd, knRBuff_remaining(conn->out_buff));
    }
    return KNEVTOK;
}

KN_HOT
static int __knServer_onPollin(
    knServer *server,
    size_t *fdIdx
)
{
    if (server->pool.pollfds[*fdIdx].fd == server->fd) {
        knInfo(server->logger, "New connection request received");
        if (knServer_accept(server) != KNEVTOK) {
            knError(server->logger, "Connection request declined");
        }
    } else {
        knInfo(server->logger, "Data received");
        switch (knServer_receiveData(server, server->pool.conns[*fdIdx])) {
            case KNEVTERR:
                knError(server->logger, "Connection [%d]: Error while receiving data", server->pool.conns[*fdIdx]->fd);
                break;
            case KNEVTKICK:
                knServer_kickAtIndex(server, *fdIdx);
                (*fdIdx)--;
                break;
            default:
                break;
        }
    }
    return KNEVTOK;
}

KN_HOT
static int __knServer_processPoll(
    knServer *server
)
{
    for (size_t i = 0; i < server->pool.count; ++i) {
        if (server->pool.pollfds[i].revents & POLLIN
            && __knServer_onPollin(server, &i) != KNEVTOK) {
                return KNEVTERR;
        }
        if (server->pool.pollfds[i].revents & POLLOUT
            && __knServer_onPollout(server, i) != KNEVTOK) {
                return KNEVTERR;
        }
    }
    return KNEVTOK;
}

KN_HOT
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
    status = poll(server->pool.pollfds, server->pool.count, timeoutMs);
    if (status == -1) {
        return KNEVTNET;
    }
    if (__knServer_processPoll(server) != KNEVTOK) {
        return KNEVTERR;
    }
    for (int i = (int)server->pool.count - 1; i >= 1; --i) {
        if (server->pool.conns[i] && server->pool.conns[i]->disconnected) {
            knServer_kickAtIndex(server, i);
        }
    }
    return KNEVTOK;
}

int knServer_run(
    knServer *server
)
{
    if (!server) {
        return KNEVTARGS;
    }
    knInfo(server->logger, "Server running on %s:%d", knServer_getIp(server), knServer_getPort(server));
    while (server->running) {
        if (knServer_runOnce(server, -1) != KNEVTOK) {
            knFatal(server->logger, "Unexpected fatal error encountered, exiting...");
            break;
        }
    }
    return KNEVTOK;
}
