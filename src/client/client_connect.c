/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include "../platform/socket.h"
#include <string.h>
#include <stdint.h>
#include "client.h"
#include "kronknet/macros/types.h"

KN_API
int knClient_connect(
    knClient *client,
    const char *ip,
    uint16_t port
)
{
    int type = SOCK_STREAM;

    if (!client || !ip || port == 0) {
        return KNEVTARGS;
    }
    knInfo(client->logger, "Connecting on %s:%d", ip, port);
    if (client->flags & knUDP) {
        type = SOCK_DGRAM;
    }
    client->fd = knSocket_open(type);
    if (client->fd == KN_INVALID_SOCKET) {
        knError(client->logger, "Error while creating socket");
        return KNEVTNET;
    }
    client->addr.sin_family = AF_INET;
    client->addr.sin_port = htons(port);
    if (inet_pton(AF_INET, (!strcmp(ip, "localhost")) ? "127.0.0.1" : ip, &client->addr.sin_addr) != 1) {
        knError(client->logger, "Error while processing ip");
        knSocket_close(client->fd);
        client->fd = KN_INVALID_SOCKET;
        return KNEVTNET;
    }
    if (connect(client->fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) == -1) {
        if (!knSocket_inProgress()) {
            knError(client->logger, "Error while connecting");
            knSocket_close(client->fd);
            client->fd = KN_INVALID_SOCKET;
            return KNEVTNET;
        }
    }
    if (client->onConnection) {
        client->onConnection(client);
    }
    return KNEVTOK;
}
