/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Receive data
*/
#include "kronknet/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>

int knServer_receiveData(knServer *server, knConnection *conn)
{
    if (!server || !conn) {
        return KNEVTARGS;
    }
    uint8_t kronkbuffer[KNBUFFSIZ] = {0};
    ssize_t reads = recv(conn->fd, kronkbuffer, KNBUFFSIZ, 0);
    if (reads > 0) {
        knServer_out(server, "Connection [%d]: %*s", conn->fd, reads, kronkbuffer);
        if (server->onRead) {
            server->onRead(conn, kronkbuffer, reads);
        }
    } else if (reads == 0) {
        knServer_err(server, "Connection [%d]: connection lost", conn->fd);
        return KNEVTKICK;
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            knServer_err(server, "Connection [%d]: connection lost", conn->fd);
            return KNEVTKICK;
        }
    }
    return KNEVTOK;
}
