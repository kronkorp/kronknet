/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include <stdlib.h>

void knClient_destroy(knClient *client)
{
    if (!client) {
        return;
    }
    knClient_clear(client);
    free(client);
}
