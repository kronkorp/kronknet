/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Get the ip of the server (but useless asf)
*/
#include "kronknet/callback/callback.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include <stdint.h>
#include "../server.h"

KN_API
knPort knServer_getPort(
    const knServer *server
)
{
    if (!server)
        return 0;
    return ntohs(server->addr.sin_port);
}
