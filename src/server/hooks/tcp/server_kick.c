/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Receive data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "../../pool/pool.h"
#include "../../server.h"
#include <stddef.h>
#include "../../../connection/connection.h"

void knServer_kick(
    knServer *server,
    knConnection *conn
)
{
    if (!server || !conn)
        return;
    for (size_t i = 0; i < server->pool.count; i++) {
        if (server->pool.conns[i] == conn) {
            knServer_kickAtIndex(server, i);
            return;
        }
    }
}

void knServer_kickAtIndex(
    knServer *server,
    size_t idx
)
{
    knConnection *conn;

    if (!server || idx == 0 || idx >= server->pool.count)
        return;
    conn = server->pool.conns[idx];
    if (server->onDisconnect) {
        server->onDisconnect(server, conn);
    }
    knInfo(server->logger, "Connection [%d]: Kicking...", conn->fd);
    // NOTE: Unregister before destroy, epoll needs the fd to be still open
    knPool_unregisterAtIndex(&server->pool, idx);
    knConnection_destroy(conn);
}
