/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onConnect callback
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"
#include "../server.h"

int knServer_setOnConnect(
    knServer *server,
    knServer_OnConnect_t callback
)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->onConnection = callback;
    return KNEVTOK;
}
