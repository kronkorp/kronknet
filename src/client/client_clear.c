/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Clear client structure
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include <arpa/inet.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

void knClient_clear(knClient *client)
{
    if (!client) {
        return;
    }
    if (client->fd != -1) {
        close(client->fd);
        client->fd = -1;
    }
}
