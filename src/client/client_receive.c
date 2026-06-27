/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Reveive data on client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client.h"

int knClient_receiveData(
    knClient *client
)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {0};

    if (!client) {
        return KNEVTARGS;
    }
    ssize_t reads = recv(client->fd, kronkbuffer, KNBUFFSIZ, 0);
    if (reads > 0) {
        knInfo(client->logger, "Received: %.*s", (int)reads, kronkbuffer);
        if (client->onRead) {
            client->onRead(client, kronkbuffer, reads);
        }
    } else if (reads == 0) {
        if (client->flags & knUDP) {
            knInfo(client->logger, "Received an empty UDP datagram");
            if (client->onRead) {
                client->onRead(client, kronkbuffer, 0);
            }
            return KNEVTOK;
        }
        knError(client->logger, "Connection lost (EOF)");
        client->running = false;
        return KNEVTKICK;
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            knError(client->logger, "Connection lost (Internal Error)");
            client->running = false;
            return KNEVTKICK;
        }
    }
    return KNEVTOK;
}