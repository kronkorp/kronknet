/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server udp destroy hook
*/
#include "../../server.h"
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include "../../../connection/connection.h"
#include "udp.h"

static void __destroy(
    uint64_t key KN_UNUSED,
    void* value,
    void* arg KN_UNUSED
)
{
    knConnection *next = NULL;

    for (knConnection *conn = (knConnection *)value; conn; conn = next) {
        next = conn->udp_next;
        knConnection_destroy(conn);
    }
}

// NOTE: The map does not own the connections (see knServer_udpAdd)
void knServer_udpDestroyHook(
    knServer* server
)
{
    knMap_foreach(server->on_udp.connections, &__destroy, NULL);
    knMap_destroy(server->on_udp.connections);
}
