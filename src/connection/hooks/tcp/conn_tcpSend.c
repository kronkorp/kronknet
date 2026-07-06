/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "../../connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

KN_API
int knConnection_tcpSendHook(
    knConnection *conn,
    const void *data,
    size_t size
)
{
    ssize_t written = 0;
    if (knRBuff_isEmpty(conn->out_buff)) {
        written = send(conn->fd, data, size, KN_NOSIGNAL);
        if (written > 0) {
            if ((size_t)written == size) {
                return KNEVTOK;
            }
        } else if (written == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                return KNEVTKICK;
            }
            written = 0;
        }
    }
    size_t remaining = size - written;
    if (knRBuff_remaining(conn->out_buff) < remaining) {
        return KNEVTKICK;
    }
    if (knRBuff_push(conn->out_buff, data + written, remaining) == -1) {
        return KNEVTERR;
    }
    knConnection_setEvents(conn, POLLOUT | POLLIN);
    return KNEVTOK;
}
