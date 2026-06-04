/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"

void *knServer_getData(const knServer *server)
{
    if (!server)
        return NULL;
    return server->data;
}
