/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Remove the connection of a UDP peer
*/
#include "../../server.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include "../../../connection/connection.h"
#include "../../../utils/address/address.h"
#include "udp.h"

// NOTE: When the first connection of a key goes, the next one takes its place in the map.
//       The key is there already, so knMap_insert only overwrites it: nothing is allocated,
//       the map is not rehashed, and with no deleter nothing is destroyed
void knServer_udpRemove(
    knServer *server,
    knConnection *conn
)
{
    uint64_t key = knAddr_hash(&conn->addr);
    knConnection *prev = knMap_search(server->on_udp.connections, key);

    if (prev == conn) {
        if (conn->udp_next) {
            knMap_insert(server->on_udp.connections, key, conn->udp_next, NULL);
        } else {
            knMap_delete(server->on_udp.connections, key);
        }
    } else {
        while (prev && prev->udp_next != conn) {
            prev = prev->udp_next;
        }
        if (prev) {
            prev->udp_next = conn->udp_next;
        }
    }
    conn->udp_next = NULL;
}
