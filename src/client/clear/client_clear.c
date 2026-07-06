/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Clear client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include <stddef.h>
#include <unistd.h>
#include "../client.h"

void knClient_clear(
    knClient *client
)
{
    if (!client) {
        return;
    }
    if (client->fd != -1) {
        close(client->fd);
        client->fd = -1;
    }
}
