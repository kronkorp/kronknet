/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onConnect callback
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/server/callback/callback.h"
#include "kronknet/server/server.h"

int knCallback_onConnection(knServer *server, knConnectionCb callback)
{
    if (!server) {
        return KNEVTARGS;
    }
    server->onConnection = callback;
    return KNEVTOK;
}
