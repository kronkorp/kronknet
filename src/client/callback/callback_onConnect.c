/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onConnect callback (for client)
*/
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "../client.h"

int knClient_setOnConnect(
    knClient *client,
    knClient_OnConnect_t callback
)
{
    if (!client) {
        return KNEVTARGS;
    }
    client->onConnection = callback;
    return KNEVTOK;
}
