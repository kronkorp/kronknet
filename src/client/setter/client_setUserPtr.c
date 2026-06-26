/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** et client data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../client.h"

void knClient_setUserPtr(
    knClient *client,
    void *user_ptr
)
{
    if (!client) {
        return;
    }
    client->user_ptr = user_ptr;
}
