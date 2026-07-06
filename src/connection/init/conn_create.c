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
#include <kronknet/macros/types.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    conn->addr_length = sizeof(conn->addr);
    conn->fd = -1;
    conn->port = ntohs(conn->addr.sin_port);
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
    const struct sockaddr_in* addr,
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
    inet_ntop(AF_INET, &conn->addr.sin_addr, conn->ip, INET_ADDRSTRLEN);
    conn->out_buff = knRBuff_create(KNBUFFSIZ);
    if (!conn->out_buff) {
        knConnection_destroy(conn);
        return NULL;
    }
    return conn;
}
