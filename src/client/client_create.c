/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Create client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

knClient *knClient_create(const char *ip, uint16_t port)
{
    knClient *client = NULL;

    if (!ip || port == 0) {
        return NULL;
    }
    client = calloc(1, sizeof(knClient));
    if (!client) {
        return NULL;
    }
    if (knClient_init(client, ip, port) != KNEVTOK) {
        knClient_destroy(client);
        return NULL;
    }
    return client;
}
