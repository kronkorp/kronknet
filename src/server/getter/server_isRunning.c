/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Is the server running ??
*/
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include <stdbool.h>
#include "../server.h"

knBool knServer_isRunning(
    const knServer *server
)
{
    if (!server)
        return false;
    return server->running;
}
