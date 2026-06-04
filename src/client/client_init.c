/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Initialize client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/poll.h>
#include <sys/socket.h>

static void __knClient_initStatic(knClient *client)
{
    client->onConnection = NULL;
    client->onRead = NULL;
    client->onWrite = NULL;
    client->onDisconnection = NULL;
    client->running = true;
    client->shouldLog = false;
    client->fd = -1;
    client->events = POLLIN;
}

int knClient_init(knClient *client)
{
    if (!client) {
        return KNEVTARGS;
    }
    __knClient_initStatic(client);
    return KNEVTOK;
}
