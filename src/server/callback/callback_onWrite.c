/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onWrite callback
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"
#include "../server.h"

int knServer_setOnWrite(
    knServer *server,
    knServer_OnWrite_t callback
)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->onWrite = callback;
    return KNEVTOK;
}
