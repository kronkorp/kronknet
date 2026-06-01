/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a connection
*/
#include "kronknet/server/pool/pool.h"
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
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
    knPool_registerFd(&server->pool, newConn->fd, POLLIN | POLLOUT);
    knPool_registerConnection(&server->pool, newConn);
    return 0;
}
