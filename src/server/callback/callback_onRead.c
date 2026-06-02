/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onRead callback
*/
#include "kronknet/connection/connection.h"
#include "kronknet/server/callback/callback.h"
#include "kronknet/server/server.h"

int knCallback_onRead(knServer *server, knReadCb callback)
{
    if (!server) {
        return -1;
    }
    server->onRead = callback;
    return 0;
}
