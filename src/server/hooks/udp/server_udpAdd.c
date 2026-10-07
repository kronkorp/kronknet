/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Add the connection of a new UDP peer
*/
#include "../../server.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include "../../../connection/connection.h"
#include "../../../utils/address/address.h"
#include "udp.h"

// NOTE: A connection whose key is taken goes at the end of the connections of that key:
//       the map keeps the first one, so it does not change
int knServer_udpAdd(
    knServer *server,
    knConnection *conn
)
{
    uint64_t key = knAddr_hash(&conn->addr);
    knConnection *last = knMap_search(server->on_udp.connections, key);

    conn->udp_next = NULL;
    if (!last) {
        if (knMap_insert(server->on_udp.connections, key, conn, NULL) == -1) {
            return KNEVTMEM;
        }
        return KNEVTOK;
    }
    while (last->udp_next) {
        last = last->udp_next;
    }
    last->udp_next = conn;
    return KNEVTOK;
}
