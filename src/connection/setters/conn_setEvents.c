/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "../connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include <stddef.h>

int knConnection_setEvents(
    knConnection *conn,
    uint32_t events
)
{
    void *ptr;

    if (!conn) {
        return KNEVTARGS;
    }
    // NOTE: In UDP, conn->fd is the server socket, so its ptr stays NULL
    ptr = (conn->flags & knUDP) ? NULL : conn;
    if (knPoller_modify(conn->poller, conn->fd, ptr, events) != KNEVTOK) {
        return KNEVTNET;
    }
    return KNEVTOK;
}
