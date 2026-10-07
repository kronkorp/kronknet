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
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/monotonic.h"
#include "udp.h"

// NOTE: value is the first connection of its key, the others follow it (see knServer_udpFind)
static void __check_timestamp(
    uint64_t key KN_UNUSED,
    void* value,
    void* arg
)
{
    knServer *s = (knServer*)arg;
    knConnection *next = NULL;

    for (knConnection *conn = (knConnection *)value; conn; conn = next) {
        next = conn->udp_next;
        if (monotonic() - conn->last_data > (timestamp)s->connection_timeout) {
            if (s->onDisconnect) {
                s->onDisconnect(s, conn);
            }
            knServer_udpRemove(s, conn);
            knConnection_destroy(conn);
        }
    }
}

void knServer_udpCleanupHook(
    knServer* server
)
{
    knMap_foreach(server->on_udp.connections, &__check_timestamp, (void *)server);
}
