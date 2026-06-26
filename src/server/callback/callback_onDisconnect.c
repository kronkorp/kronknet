/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onDisconection callback
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"
#include "../server.h"

int knServer_setOnDisconnect(
    knServer *server,
    knServer_OnDisconnect_t callback
)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->onDisconnect = callback;
    return KNEVTOK;
}
