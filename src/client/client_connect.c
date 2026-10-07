/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include "../platform/socket.h"
#include "../utils/address/address.h"
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
    // NOTE: The family of the socket is the one of the ip (IPv4 or IPv6), so the ip comes first
    if (knAddr_fromIp(&client->addr, (!strcmp(ip, "localhost")) ? "127.0.0.1" : ip, port) != KNEVTOK) {
        knError(client->logger, "Error while processing ip");
        return KNEVTNET;
    }
    client->fd = knSocket_open(client->addr.any.sa_family, type);
    if (client->fd == KN_INVALID_SOCKET) {
        knError(client->logger, "Error while creating socket");
        return KNEVTNET;
    }
    if (connect(client->fd, &client->addr.any, client->addr.len) == -1) {
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
