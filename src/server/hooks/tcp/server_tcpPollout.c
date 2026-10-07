/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server tcp pollout hook
*/
#include <stddef.h>
#include <sys/epoll.h>
#include "kronknet/macros/errdef.h"
#include "../../server.h"
#include "../../../connection/connection.h"
#include "../../pool/pool.h"
#include "../../../platform/socket.h"

int knServer_tcpPolloutHook(
    knServer* server,
    knConnection *conn
)
{
    uint8_t tmp[KNBUFFSIZ] = {0};
    size_t usage = knRBuff_usage(conn->out_buff);

    knInfo(server->logger, "Connection [%zu]: Attempting to send some data", conn->id);
    knRBuff_peek(conn->out_buff, tmp, usage);
    ssize_t sends = knSocket_send(conn->fd, tmp, usage);
    if (sends > 0) {
        knRBuff_pop(conn->out_buff, NULL, sends);
        knInfo(server->logger, "Connection [%zu]: sent %zu bytes, remaining: %zu bytes.", conn->id, (size_t)sends, knRBuff_usage(conn->out_buff));
        if (knRBuff_isEmpty(conn->out_buff)) {
            knConnection_setEvents(conn, EPOLLIN);
            if (server->onWrite) {
                server->onWrite(conn);
            }
        }
    } else {
        knError(server->logger, "Connection [%zu]: Failed to send data, remaining: %zu bytes.", conn->id, knRBuff_remaining(conn->out_buff));
    }
    return KNEVTOK;
}
