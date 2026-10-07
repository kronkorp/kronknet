/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Find the connection of a UDP peer
*/
#include "../../server.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include "../../../connection/connection.h"
#include "../../../utils/address/address.h"
#include "udp.h"

knConnection *knServer_udpFind(
    const knServer *server,
    const knAddr *addr
)
{
    knConnection *conn = knMap_search(server->on_udp.connections, knAddr_hash(addr));

    while (conn && !knAddr_equal(&conn->addr, addr)) {
        conn = conn->udp_next;
    }
    return conn;
}
