/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server udp cleanup hook
*/
#include "../../server.h"
#include "kronknet/callback/callback.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stdint.h>
#include "../../../connection/connection.h"
#include "kronknet/utils/monotonic.h"

static void __check_timestamp(
    uint64_t key,
    void* value,
    void* arg
)
{
    knServer *s = (knServer*)arg;
    knConnection *conn = (knConnection *)value;

    if (monotonic() - conn->last_data > (timestamp)s->connection_timeout) {
        if (s->onDisconnect) {
            s->onDisconnect(s, conn);
        }
        knMap_delete(s->on_udp.connections, key);
        knConnection_destroy(conn);
    }
}

void knServer_udpCleanupHook(
    knServer* server
)
{
    knMap_foreach(server->on_udp.connections, &__check_timestamp, (void *)server);
}
