/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include <string.h>
#include <stdint.h>
#include "client.h"
#include "kronknet/macros/types.h"
#include <unistd.h>

static int __set_socket_non_blocking(
    knClient *client
)
{
    #ifndef _WIN32
        int flags = fcntl(client->fd, F_GETFL, 0);

        if (flags != -1) {
            if (fcntl(client->fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                return -1;
            }
        } else {
            return -1;
        }
    #else
        u_long mode = 1;
        if (ioctlsocket(client->fd, FIONBIO, &mode) != 0) {
            return -1;
        }
    return 0;
    #endif /* _WIN32 */
}

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
    client->fd = socket(AF_INET, type, 0);
    if (client->fd == -1) {
        knError(client->logger, "Error while creating socket");
        return KNEVTNET;
    }
    __set_socket_non_blocking(client);
    client->addr.sin_family = AF_INET;
    client->addr.sin_port = htons(port);
    if (inet_pton(AF_INET, (!strcmp(ip, "localhost")) ? "127.0.0.1" : ip, &client->addr.sin_addr) != 1) {
        knError(client->logger, "Error while processing ip");
        close(client->fd);
        return KNEVTNET;
    }
    if (connect(client->fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) == -1) {
        if (errno != EINPROGRESS) {
            knError(client->logger, "Error while connecting");
            close(client->fd);
            return KNEVTNET;
        }
    }
    if (client->onConnection) {
        client->onConnection(client);
    }
    return KNEVTOK;
}
