/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Reveive data on client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>

int knClient_receiveData(knClient *client)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {};

    if (!client) {
        return KNEVTARGS;
    }
    ssize_t reads = recv(client->fd, kronkbuffer, KNBUFFSIZ, 0);
    if (reads > 0) {
        knClient_out(client, "Received: %.*s", reads, kronkbuffer);
        if (client->onRead) {
            client->onRead(client, kronkbuffer, reads);
        }
    } else if (reads == 0) {
        knClient_err(client, "Connection lost");
        client->running = false;
        return KNEVTKICK;
    } else {
        if (errno != EAGAIN || errno != EWOULDBLOCK) {
            knClient_err(client, "Connection lost");
            client->running = false;
            return KNEVTKICK;
        }
    }
    return KNEVTOK;
}
