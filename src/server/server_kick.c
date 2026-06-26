/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Receive data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "pool/pool.h"
#include "server.h"
#include <stddef.h>
#include "../connection/connection.h"

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
    if (!server || idx == 0)
        return;
    if (server->onDisconnect) {
        server->onDisconnect(server, server->pool.conns[idx]);
    }
    knInfo(server->logger, "Connection [%d]: Kicking...", server->pool.conns[idx]->fd);
    knConnection_destroy(server->pool.conns[idx]);
    knPool_unregisterAtIndex(&server->pool, idx);
}
