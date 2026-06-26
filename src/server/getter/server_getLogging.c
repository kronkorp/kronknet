/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get server logs related
*/
#include "kronknet/callback/callback.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include "../server.h"
#include <stdio.h>

knLogLevel knServer_getLogLevel(
    const knServer *server
)
{
    if (!server)
        return knLogNone;
    return server->logger.log_level;
}

FILE *knServer_getLogOutput(
    const knServer *server
)
{
    if (!server)
        return NULL;
    return server->logger.out;
}
