/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set onDisconection callback
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../client.h"

int knClient_setOnDisconnect(
    knClient *client,
    knClient_OnDisconnect_t callback
)
{
    if (!client) {
        return KNEVTARGS;
    }
    client->onDisconnect = callback;
    return KNEVTOK;
}
