/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Receive data
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include "server.h"
#include <errno.h>
#include <stdint.h>
#include <sys/socket.h>
#include "../connection/connection.h"

int knServer_receiveData(
    knServer *server,
    knConnection *conn
)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {0};

    if (!server || !conn) {
        return KNEVTARGS;
    }
    ssize_t reads = recv(conn->fd, kronkbuffer, sizeof(kronkbuffer), 0);
    if (reads > 0) {
        knInfo(server->logger, "Connection [%d] sends %zd bytes", conn->fd, reads);
        if (server->onRead) {
            server->onRead(conn, kronkbuffer, reads);
        }
    } else if (reads == 0) {
        knError(server->logger, "Connection [%d]: connection lost", conn->fd);
        return KNEVTKICK;
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            knError(server->logger, "Connection [%d]: connection lost", conn->fd);
            return KNEVTKICK;
        }
    }
    return KNEVTOK;
}
