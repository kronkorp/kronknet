/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a connection
*/
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "pool/pool.h"
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../utils/logger/logger.h"
#include "server.h"
#include "../connection/connection.h"

int knServer_accept(
    knServer *server
)
{
    knConnection *newConn = NULL;

    if (!server)
        return KNEVTARGS;
    newConn = knConnection_accept(server);
    if (!newConn)
        return KNEVTERR;
    knInfo(server->logger, "Connection [%d] from %s:%d", newConn->fd, newConn->ip, newConn->port);
    if (knPool_registerFd(&server->pool, newConn->fd, newConn, POLLIN) != KNEVTOK) {
            knError(server->logger, "Connection [%d]: failed to add to pool", newConn->fd);
            return KNEVTERR;
    }
    knInfo(server->logger, "Connection [%d]: added to pool", newConn->fd);
    if (server->onConnection) {
        switch (server->onConnection(server, newConn)) {
            case KNEVTOK:
                break;
            default:
                knError(server->logger, "Connection [%d]: Error on \"onConnection\" callback", newConn->fd);
                knServer_kick(server, newConn);
                return KNEVTKICK;
        }
    }
    return KNEVTOK;
}
