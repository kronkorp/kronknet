/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/optimization.h"
#include "kronknet/macros/types.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdbool.h>
#include <stddef.h>
#include "client.h"

static int __knClient_onPollout(
    knClient *client
)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {};
    size_t usage = knRBuff_usage(client->buff);

    if (usage == 0) return KNEVTOK;

    knInfo(client->logger, "Attempting to send some data from ring buffer");
    knRBuff_peek(client->buff, kronkbuffer, usage);
    ssize_t sends = send(client->fd, kronkbuffer, usage, KN_NOSIGNAL);
    if (sends > 0) {
        knRBuff_pop(client->buff, NULL, sends);
        knInfo(client->logger, "Sent %zu bytes, remaining: %zu bytes.", (size_t)sends, knRBuff_usage(client->buff));
        if (knRBuff_isEmpty(client->buff)) {
            if (client->onWrite) {
                client->onWrite(client);
            }
        }
    } else {
        knWarn(client->logger, "Failed to send data or buffer full, remaining: %zu bytes.", knRBuff_usage(client->buff));
    }
    return KNEVTOK;
}

KN_API
KN_HOT
int knClient_runOnce(
    knClient *client,
    ssize_t timeout
)
{
    struct pollfd p;
    int status = 0;

    if (!client) {
        return KNEVTARGS;
    }
    p = (struct pollfd){client->fd, client->events, 0};
    if (!knRBuff_isEmpty(client->buff)) {
        p.events |= POLLOUT;
    }
    #ifndef _WIN32
        status = poll(&p, 1, timeout);
    #else
        status = WSAPoll(&p, 1, timeout);
    #endif /* _WIN32 */
    if (status == -1) {
        return KNEVTNET;
    }
    if (p.revents & POLLIN) {
        knInfo(client->logger, "Data received");
        knClient_receiveData(client);
    }
    if (p.revents & POLLOUT) {
        __knClient_onPollout(client);
    }
    if (p.revents & (POLLHUP | POLLERR | POLLNVAL)) {
        if (client->onDisconnect) {
            client->onDisconnect(client);
        }
        client->running = false;
        return KNEVTKICK;
    }
    return KNEVTOK;
}

KN_API
int knClient_run(
    knClient *client
)
{
    int res;

    if (!client) {
        return KNEVTARGS;
    }
    knInfo(client->logger, "Cient (mode=%s) running",
        (client->flags & knTCP) ? "TCP" : "UDP");
    while (client->running) {
        res = knClient_runOnce(client, -1);
        if (res == KNEVTKICK) {
            break;
        }
        if (res != KNEVTOK) {
            return KNEVTERR;
        }
    }
    return KNEVTOK;
}
