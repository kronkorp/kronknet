/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a connection
*/
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
        return -1;
    }
    newConn = knConnection_accept(server);
    if (!newConn) {
        return -1;
    }
    if (knPool_registerFd(&server->pool, newConn->fd, POLLIN) == -1 ||
        knPool_registerConnection(&server->pool, newConn) == -1) {
            knServer_err(server, "Connection [%d]: failed to add to pool", newConn->fd);
            return -1;
    }
    knServer_out(server, "Connection [%d]: added to pool", newConn->fd);
    if (server->onConnection) {
        if (server->onConnection(server, newConn) == -1) {
            knServer_err(server, "Connetction [%d]: Error on \"onConnection\" callback", newConn->fd);
            knServer_kick(server, newConn);
            return KNEVTKICK;
        }
    }
    return 0;
}
