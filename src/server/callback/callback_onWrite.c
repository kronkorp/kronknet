/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onWrite callback
*/
#include "kronknet/connection/connection.h"
#include "kronknet/server/callback/callback.h"
#include "kronknet/server/server.h"

int knCallback_onWrite(knServer *server, knEventCb callback)
{
    if (!server) {
        return -1;
    }
    server->onWrite = callback;
    return 0;
}
