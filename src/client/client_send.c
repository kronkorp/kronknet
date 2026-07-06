/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send data to server
*/
#include "kronknet/callback/callback.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "kronknet/client/client.h"
#include <errno.h>
#include <stddef.h>
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

#ifdef _WIN32
    int written = 0;
    int flags = 0;
    int size_to_send = (size > INT_MAX) ? INT_MAX : (int)size;
#else
    ssize_t written = 0;
    int flags = KN_NOSIGNAL;
    size_t size_to_send = size;
#endif

    const uint8_t *byte_ptr = (const uint8_t *)data;

    if (knRBuff_isEmpty(client->buff)) {
        written = send(client->fd, (const char *)data, size_to_send, flags);
        
        if (written > 0) {
            if ((size_t)written == size) {
                return KNEVTOK;
            }
        } else if (written == -1) {
#ifdef _WIN32
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK) {
                return KNEVTKICK;
            }
#else
            if (errno != EAGAIN && errno != EWOULDBLOCK)  {
                return KNEVTKICK;
            }
#endif
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
    
    client->events |= POLLOUT;
    return KNEVTOK;
}