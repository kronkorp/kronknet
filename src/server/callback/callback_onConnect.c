/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onConnect callback
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"

int knServer_onConnectionCallback(knServer *server, knConnectionCb callback)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->onConnection = callback;
    return KNEVTOK;
}
