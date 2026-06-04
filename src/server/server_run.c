/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the server. main loop.
*/
#include "kronknet/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>

static int __knServer_onPollout([[maybe_unused]] knServer *server, [[maybe_unused]] size_t fdIdx)
{
    uint8_t tmp[KNBUFFSIZ] = {};
    knConnection *conn = server->pool.conns[fdIdx];

    // FIXME: Peek -> consume
    size_t usage = knRBuff_usage(conn->out_buff);
    knServer_out(server, "Connection [%d]: Attempting to send some data", conn->fd);
    knRBuff_pop(conn->out_buff, tmp, usage);
    ssize_t sends = send(conn->fd, tmp, usage, MSG_NOSIGNAL);
    if (sends > 0) {
        knServer_out(server, "Connection [%d]: sent %zu bytes, remaining: %zu bytes.", conn->fd, (size_t)sends, knRBuff_usage(conn->out_buff));
        if (knRBuff_isEmpty(conn->out_buff)) {
            knConnection_setEvents(server->pool.conns[fdIdx], POLLIN);
            if (server->onWrite) {
                server->onWrite(conn);
            }
        }
    } else {
        knServer_err(server, "Connection [%d]: Failed to send data, remaining: %zu bytes.", conn->fd, knRBuff_remaining(conn->out_buff));
    }
    return KNEVTOK;
}

static int __knServer_onPollin([[maybe_unused]] knServer *server, [[maybe_unused]] size_t *fdIdx)
{
    if (server->pool.pollfds[*fdIdx].fd == server->fd) {
        knServer_out(server, "New connection request received");
        if (knServer_accept(server) == -1) {
            knServer_err(server, "Connection request declined");
        }
    } else {
        knServer_out(server, "Data received");
        switch (knServer_receiveData(server, server->pool.conns[*fdIdx])) {
            case KNEVTERR:
                knServer_err(server, "Connection [%d]: Error while receiving data", server->pool.conns[*fdIdx]->fd);
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

static int __knServer_processPoll(knServer *server)
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

int knServer_runOnce(knServer *server, ssize_t timeoutMs)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->status = poll(server->pool.pollfds, server->pool.count, timeoutMs);
    if (server->status == -1) {
        return KNEVTNET;
    }
    if (__knServer_processPoll(server) == -1) {
        return KNEVTERR;
    }
    return KNEVTOK;
}

int knServer_run(knServer *server)
{
    if (!server) {
        return KNEVTARGS;
    }
    knServer_out(server, "Server running on port %d", ntohs(server->addr.sin_port));
    while (server->running) {
        if (knServer_runOnce(server, -1) != KNEVTOK) {
            break;
        }
    }
    return KNEVTOK;
}
