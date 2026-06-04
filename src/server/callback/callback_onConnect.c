/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onConnect callback (for client)
*/
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include "kronknet/callback/callback.h"

int knClient_onConnectionCallback(knClient *client, knClientCb callback)
{
    if (!client) {
        return KNEVTARGS;
    }
    client->onConnection = callback;
    return KNEVTOK;
}
