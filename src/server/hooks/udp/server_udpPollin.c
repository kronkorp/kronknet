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
#include <sys/poll.h>
#include "../../../connection/connection.h"
#include "kronknet/utils/monotonic.h"
#include "../../../server/server.h"

int knServer_udpPollinHook(
    knServer* server,
    size_t *idx KN_UNUSED
)
{
    struct sockaddr_in addr = {};
    socklen_t addr_len = sizeof(addr);
    uint8_t buffer[KNBUFFSIZ] = {0};
    knConnection *conn = NULL;
    ssize_t reads = recvfrom(server->fd, buffer, sizeof(buffer),
        0, (struct sockaddr *)&addr, &addr_len);

    if (reads < 0) {
        return KNEVTOK;
    }
    uint64_t key = ((uint64_t)addr.sin_addr.s_addr << 32 | (uint64_t)addr.sin_port);
    conn = knMap_search(server->on_udp.connections, key);
    if (!conn) {
        conn = knConnection_create(&addr, server->flags);
        conn->fd = server->fd;
        conn->evtptr = &server->pool.pollfds[0].events;
        if (!conn)
            return KNEVTMEM;
        if (knMap_insert(server->on_udp.connections, key,
            conn, (knMapDeleter)&knConnection_destroy) == -1)
            return KNEVTERR;
        if (server->onConnection)
            server->onConnection(server, conn);
    }
    conn->last_data = monotonic();
    if (server->onRead)
        server->onRead(conn, buffer, (size_t)reads);
    return KNEVTOK;
}
