/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a connection
*/
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/pool/pool.h"
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

int knServer_accept(knServer *server)
{
    knConnection *newConn = NULL;

    if (!server) {
        return KNEVTARGS;
    }
    newConn = knConnection_accept(server);
    if (!newConn) {
        return KNEVTERR;
    }
    if (knPool_registerFd(&server->pool, newConn->fd, POLLIN) != KNEVTOK ||
        knPool_registerConnection(&server->pool, newConn) != KNEVTOK) {
            knServer_err(server, "Connection [%d]: failed to add to pool", newConn->fd);
            return KNEVTERR;
    }
    knServer_out(server, "Connection [%d]: added to pool", newConn->fd);
    if (server->onConnection) {
        switch (server->onConnection(server, newConn)) {
            case KNEVTOK:
                break;
            default:
                knServer_err(server, "Connection [%d]: Error on \"onConnection\" callback, kicking", newConn->fd);
                knServer_kick(server, newConn);
                break;
        }
    }
    return 0;
}
