/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server tcp pollin hook
*/
#include "../../../platform/socket.h"
#include <stddef.h>
#include "kronknet/macros/errdef.h"
#include "../../server.h"
#include "../../../connection/connection.h"
#include "../../pool/pool.h"
#include "kronknet/utils/monotonic.h"

static int __knServer_accept(
    knServer *server
)
{
    knConnection *newConn = NULL;

    if (!server)
        return KNEVTARGS;
    newConn = knConnection_accept(server);
    if (!newConn)
        return KNEVTERR;
    knInfo(server->logger, "Connection [%zu] from %s:%d", newConn->id, newConn->ip, newConn->port);
    if (knPool_registerFd(&server->pool, newConn->fd, newConn, KN_POLLIN) != KNEVTOK) {
            knError(server->logger, "Connection [%zu]: failed to add to pool", newConn->id);
            knConnection_destroy(newConn);
            return KNEVTERR;
    }
    knInfo(server->logger, "Connection [%zu]: added to pool", newConn->id);
    if (server->onConnection) {
        switch (server->onConnection(server, newConn)) {
            case KNEVTOK:
                break;
            default:
                knError(server->logger, "Connection [%zu]: Error on \"onConnection\" callback", newConn->id);
                knServer_kick(server, newConn);
                return KNEVTKICK;
        }
    }
    return KNEVTOK;
}

static int __knServer_receiveData(
    knServer *server,
    knConnection *conn
)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {0};

    if (!server || !conn) {
        return KNEVTARGS;
    }
    ssize_t reads = knSocket_recv(conn->fd, kronkbuffer, sizeof(kronkbuffer));
    if (reads > 0) {
        knInfo(server->logger, "Connection [%zu] sends %zd bytes", conn->id, reads);
        conn->last_data = monotonic();
        if (server->onRead) {
            server->onRead(conn, kronkbuffer, reads);
        }
    } else if (reads == 0) {
        knError(server->logger, "Connection [%zu]: connection lost", conn->id);
        return KNEVTKICK;
    } else {
        if (!knSocket_wouldBlock()) {
            knError(server->logger, "Connection [%zu]: connection lost", conn->id);
            return KNEVTKICK;
        }
    }
    return KNEVTOK;
}


int knServer_tcpPollinHook(
    knServer* server,
    knConnection *conn
)
{
    if (!conn) {
        knInfo(server->logger, "New connection request received");
        if (__knServer_accept(server) != KNEVTOK) {
            knError(server->logger, "Connection request declined");
        }
    } else {
        knInfo(server->logger, "Data received");
        switch (__knServer_receiveData(server, conn)) {
            case KNEVTERR:
                knError(server->logger, "Connection [%zu]: Error while receiving data", conn->id);
                break;
            case KNEVTKICK:
                knServer_kick(server, conn);
                return KNEVTKICK;
            default:
                break;
        }
    }
    return KNEVTOK;
}
