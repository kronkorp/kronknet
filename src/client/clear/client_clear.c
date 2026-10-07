/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Clear client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../../platform/socket.h"
#include <stddef.h>
#include "../client.h"

void knClient_clear(
    knClient *client
)
{
    if (!client) {
        return;
    }
    knSocket_close(client->fd);
    client->fd = KN_INVALID_SOCKET;
    knRBuff_destroy(client->buff);
    client->buff = NULL;
}
