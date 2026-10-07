/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Accept a new connection
*/
#include "../connection.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include "../../platform/socket.h"
#include "../../utils/address/address.h"
#include <kronknet/macros/types.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "kronknet/utils/monotonic.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "../../server/server.h"
#include "../hooks/tcp/tcp.h"
#include "../hooks/udp/udp.h"

static void __knConnection_statics(
    knConnection *conn
)
{
    static size_t id = 0;

    conn->fd = KN_INVALID_SOCKET;
    conn->poller = NULL;
    conn->udp_next = NULL;
    conn->port = knAddr_getPort(&conn->addr);
    conn->id = id++;
    conn->last_data = monotonic();
    conn->disconnected = false;
}

static void __knConnection_hooks(
    knConnection *conn
)
{
    conn->sendHook = NULL;
    if (conn->flags & knTCP) {
        conn->sendHook = &knConnection_tcpSendHook;
    } else if (conn->flags & knUDP) {
        conn->sendHook = &knConnection_udpSendHook;
    }
}

knConnection *knConnection_create(
    const knAddr* addr,
    knFlags flags
)
{
    knConnection *conn = calloc(1, sizeof(knConnection));

    if (!conn)
        return NULL;
    conn->flags = flags;
    conn->addr = *addr;
    __knConnection_statics(conn);
    __knConnection_hooks(conn);
    knAddr_toIp(&conn->addr, conn->ip, sizeof(conn->ip));
    conn->out_buff = knRBuff_create(KNBUFFSIZ);
    if (!conn->out_buff) {
        knConnection_destroy(conn);
        return NULL;
    }
    return conn;
}
