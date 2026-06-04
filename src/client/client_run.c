/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

static int __knClient_onPollout(knClient *client)
{
    uint8_t kronkbuffer[KNBUFFSIZ] = {};

    size_t usage = knRBuff_usage(client->buff);
    knClient_out(client, "Attempting to send some data from ring buffer");
    knRBuff_pop(client->buff, kronkbuffer, usage);
    ssize_t sends = send(client->fd, kronkbuffer, usage, MSG_NOSIGNAL);
    if (sends > 0) {
        knClient_out(client, "Sent %zu bytes, remaining: %zu bytes.", (size_t)sends, knRBuff_usage(client->buff));
        if (knRBuff_isEmpty(client->buff)) {
            if (client->onWrite) {
                client->onWrite(client);
            }
        }
    } else {
        knClient_err(client, "Failed to send data, remaining: %zu bytes.", knRBuff_remaining(client->buff));
    }
    return KNEVTOK;
}

int knClient_runOnce(knClient *client, ssize_t timeout)
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
    status = poll(&p, 1, timeout);
    if (status == -1) {
        return KNEVTNET;
    }
    if (p.revents & POLLIN) {
        knClient_out(client, "POLLIN spotted!!!");
        knClient_receiveData(client);
    }
    if (p.revents & POLLOUT) {
        __knClient_onPollout(client);
    }
    if (p.revents & (POLLHUP | POLLERR)) {
        if (client->onDisconnection) {
            client->onDisconnection(client);
        }
        client->running = false;
        return KNEVTKICK;
    }
    return KNEVTOK;
}

int knClient_run(knClient *client)
{
    if (!client) {
        return KNEVTARGS;
    }
    while (client->running) {
        if (knClient_runOnce(client, -1) != KNEVTOK) {
            return KNEVTERR;
        }
    }
    return KNEVTOK;
}
