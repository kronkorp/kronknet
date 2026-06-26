/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Stop the server
*/
#include "kronknet/callback/callback.h"
#include "../server.h"
#include <stdbool.h>

void knServer_stop(
    knServer *server
)
{
    if (!server)
        return;
    server->running = false;
}
