/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server udp pollin hook
*/
#include "../../server.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/optimization.h"
#include <stddef.h>
#include "../../../connection/connection.h"
#include "kronknet/utils/monotonic.h"
#include "../../../server/server.h"
#include "../../../platform/socket.h"
#include "udp.h"

int knServer_udpPollinHook(
    knServer* server,
    knConnection *evtconn KN_UNUSED
)
{
    knAddr addr = {0};
    uint8_t buffer[KNBUFFSIZ] = {0};
    knConnection *conn = NULL;
    ssize_t reads = knSocket_recvFrom(server->fd, buffer, sizeof(buffer), &addr);

    if (reads < 0) {
        return KNEVTOK;
    }
    conn = knServer_udpFind(server, &addr);
    if (!conn) {
        conn = knConnection_create(&addr, server->flags);
        if (!conn)
            return KNEVTMEM;
        conn->fd = server->fd;
        conn->poller = server->pool.poller;
        if (knServer_udpAdd(server, conn) != KNEVTOK) {
            knConnection_destroy(conn);
            return KNEVTERR;
        }
        if (server->onConnection)
            server->onConnection(server, conn);
    }
    conn->last_data = monotonic();
    if (server->onRead)
        server->onRead(conn, buffer, (size_t)reads);
    return KNEVTOK;
}
