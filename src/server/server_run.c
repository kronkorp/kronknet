/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the server. main loop.
*/
#include "kronknet/server/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/types.h>

static int __knServer_onPollout([[maybe_unused]] knServer *server, [[maybe_unused]] size_t fdIdx)
{
    return 0;
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
    return 0;
}

static int __knServer_processPoll(knServer *server)
{
    for (size_t i = 0; i < server->pool.count; ++i) {
        if (server->pool.pollfds[i].revents & POLLIN
            && __knServer_onPollin(server, &i) == -1) {
                return -1;
        }
        if (server->pool.pollfds[i].revents & POLLOUT
            && __knServer_onPollout(server, i) == -1) {
                return -1;
        }
    }
    return 0;
}

int knServer_runOnce(knServer *server, ssize_t timeoutMs)
{
    if (!server) {
        return -1;
    }
    server->status = poll(server->pool.pollfds, server->pool.count, timeoutMs);
    if (server->status == -1) {
        return -1;
    }
    if (__knServer_processPoll(server) == -1) {
        return -1;
    }
    return 0;
}

int knServer_run(knServer *server)
{
    if (!server) {
        return -1;
    }
    knServer_out(server, "Server running on port %d", ntohs(server->addr.sin_port));
    while (server->running) {
        if (knServer_runOnce(server, -1) == -1) {
            break;
        }
    }
    return server->status;
}
