/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Create the server
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include <stddef.h>
#include <stdlib.h>
#include "../server.h"

knServer *knServer_create(
    knPort port
)
{
    knServer *server = calloc(1, sizeof(knServer));

    if (!server) {
        return NULL;
    }
    if (knServer_init(server, port) != KNEVTOK) {
        knServer_destroy(server);
        return NULL;
    }
    return server;
}
