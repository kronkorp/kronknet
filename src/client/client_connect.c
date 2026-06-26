/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>
#include "client.h"

int knClient_connect(
    knClient *client,
    const char *ip,
    uint16_t port
)
{
    if (!client || !ip || port == 0) {
        return KNEVTARGS;
    }
    knInfo(client->logger, "Connecting on %s:%d", ip, port);
    client->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->fd == -1) {
        knError(client->logger, "Error while creating socket");
        return KNEVTNET;
    }
    client->addr.sin_family = AF_INET;
    client->addr.sin_port = htons(port);
    if (inet_pton(AF_INET, (!strcmp(ip, "localhost")) ? "127.0.0.1" : ip, &client->addr.sin_addr) != 1) {
        knError(client->logger, "Error while processing ip");
        return KNEVTNET;
    }
    if (connect(client->fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) != KNEVTOK) {
        knError(client->logger, "Error while connecting");
        return KNEVTNET;
    }
    if (client->onConnection)
        client->onConnection(client);
    return KNEVTOK;
}
