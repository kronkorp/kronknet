/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** et client data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include <stdbool.h>

void knClient_close(knClient *client)
{
    if (!client) {
        return;
    }
    client->running = false;
}
