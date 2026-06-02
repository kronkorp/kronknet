/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/server/callback/callback.h"
#include "kronknet/server/server.h"

void knServer_setData(knServer *server, void *data)
{
    if (!server)
        return;
    server->data = data;
}
