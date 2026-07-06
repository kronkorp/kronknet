/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Initialize client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../client.h"
#include "kronknet/macros/types.h"

static void __knClient_initStatic(
    knClient *client
)
{
    client->onConnection = NULL;
    client->onRead = NULL;
    client->onWrite = NULL;
    client->onDisconnect = NULL;
    client->running = true;
    client->fd = -1;
    client->events = POLLIN;
    client->logger = (knLoggerData){
        .log_level = knLogNone,
        .out = NULL,
    };
}

int knClient_init(
    knClient *client,
    knFlags flags
)
{
    if (!client) {
        return KNEVTARGS;
    }
    __knClient_initStatic(client);
    client->flags = flags;
    return KNEVTOK;
}
