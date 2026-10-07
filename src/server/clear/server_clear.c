/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Clear the server
*/
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include "../../platform/socket.h"
#include "../pool/pool.h"
#include "../server.h"
#include "kronknet/utils/hashmap/hashmap.h"

void knServer_clear(
    knServer *server
)
{
    if (!server) {
        return;
    }
    knSocket_close(server->fd);
    server->fd = KN_INVALID_SOCKET;
    if (server->onDestroyHook) {
        server->onDestroyHook(server);
    }
    knPool_clear(&server->pool);
}
