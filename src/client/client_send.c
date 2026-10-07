/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send data to server
*/
#include "kronknet/callback/callback.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "../platform/socket.h"
#include <stddef.h>
#include "kronknet/client/client.h"
#include "client.h"

KN_API
int knClient_sendServer(
    knClient *client,
    const void *data,
    size_t size
)
{
    if (!client || !data || size == 0) {
        return KNEVTARGS;
    }
    ssize_t written = 0;
    const uint8_t *byte_ptr = (const uint8_t *)data;
    if (client->flags & knUDP) {
        // A datagram is sent whole, now, or dropped when the socket is full (see knConnection_udpSendHook)
        written = knSocket_send(client->fd, data, size);
        if (written == -1 && !knSocket_wouldBlock()) {
            return KNEVTKICK;
        }
        return KNEVTOK;
    }
    if (knRBuff_isEmpty(client->buff)) {
        written = knSocket_send(client->fd, data, size);
        if (written > 0) {
            if ((size_t)written == size) {
                return KNEVTOK;
            }
        } else if (written == -1) {
            if (!knSocket_wouldBlock()) {
                return KNEVTKICK;
            }
            written = 0;
        }
    }
    size_t remaining = size - written;
    if (knRBuff_remaining(client->buff) < remaining) {
        return KNEVTKICK;
    }
    if (knRBuff_push(client->buff, byte_ptr + written, remaining) == -1) {
        return KNEVTERR;
    }
    
    client->events |= KN_POLLOUT;
    return KNEVTOK;
}
