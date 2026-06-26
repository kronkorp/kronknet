/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** et client data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/types.h"
#include <stdbool.h>
#include "../client.h"

void knClient_close(
    knClient *client
)
{
    if (!client) {
        return;
    }
    client->running = knFalse;
}
