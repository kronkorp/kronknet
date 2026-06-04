/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Initialize client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include <arpa/inet.h>
#include <stddef.h>
#include <sys/socket.h>

int knClient_init(knClient *client, const char *ip, uint16_t port)
{
    if (!client || !ip || port == 0) {
        return KNEVTARGS;
    }
    client->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->fd == -1) {
        return KNEVTNET;
    }
    if (inet_pton(AF_INET, ip, &client->addr.sin_addr) != KNEVTOK) {
        return KNEVTNET;
    }
    if (connect(client->fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) != KNEVTOK) {
        return KNEVTNET;
    }
    return KNEVTOK;
}
