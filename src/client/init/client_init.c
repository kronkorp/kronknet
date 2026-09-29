/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Initialize client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/socket.h>
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
    // Where what the socket cannot take waits (knClient_sendServer). A datagram is never kept: UDP has none.
    if (!(flags & knUDP)) {
        client->buff = knRBuff_create(KNBUFFSIZ);
        if (!client->buff) {
            return KNEVTMEM;
        }
    }
    return KNEVTOK;
}
