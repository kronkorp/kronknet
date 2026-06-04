/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Send data to server
*/
#include "kronknet/callback/callback.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stddef.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "kronknet/client/client.h"

int knClient_sendServer(knClient *client, void *data, size_t size)
{
    if (!client || !data || size == 0) {
        return KNEVTARGS;
    }
    ssize_t written = 0;
    if (knRBuff_isEmpty(client->buff)) {
        written = send(client->fd, data, size, MSG_NOSIGNAL);
        if (written > 0) {
            if ((size_t)written == size) {
                return KNEVTOK;
            }
        } else if (written == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK)  {
                return KNEVTKICK;
            }
            written = 0;
        }
    }
    size_t remaining = size - written;
    if (knRBuff_remaining(client->buff) < remaining) {
        return KNEVTKICK;
    }
    if (knRBuff_push(client->buff, data + written, remaining) == -1) {
        return KNEVTERR;
    }
    client->events |= POLLOUT;
    return KNEVTOK;
}
