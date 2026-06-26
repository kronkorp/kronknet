/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../client.h"

void *knClient_getUserPtr(
    const knClient *client
)
{
    if (!client) {
        return NULL;
    }
    return client->user_ptr;
}
