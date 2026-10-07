/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Accept a new connection
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include "../../platform/socket.h"
#include <stdbool.h>
#include <stddef.h>
#include "../../server/server.h"

knConnection *knConnection_accept(
    const knServer *server
)
{
    knConnection *conn;
    knSocket fd;
    struct sockaddr_in addr;

    fd = knSocket_accept(server->fd, &addr);
    if (fd == KN_INVALID_SOCKET) {
        return NULL;
    }
    conn = knConnection_create(&addr, server->flags);
    if (!conn) {
        knSocket_close(fd);
        return NULL;
    }
    conn->fd = fd;
    return conn;
}
