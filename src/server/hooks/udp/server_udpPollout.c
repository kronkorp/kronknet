/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server udp pollout hook
*/
#include "../../server.h"
#include "kronknet/callback/callback.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/optimization.h"
#include <stddef.h>
#include <stdint.h>
#include "kronknet/utils/hashmap/hashmap.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "../../../connection/connection.h"

typedef struct {

    knServer *server;
    knBool packets_remaining;

} knPolloutContext;

static void __pollout(
    uint64_t key KN_UNUSED,
    void* value,
    void* arg
)
{
    knConnection *conn = (knConnection *)value;
    knPolloutContext *ctx = (knPolloutContext *)arg;
    knServer* server = ctx->server;
    uint8_t tmp[KNBUFFSIZ] = {};
    size_t usage = knRBuff_usage(conn->out_buff);

    if (usage == 0)
        return;
    knInfo(server->logger, "Client [%zu] (%s:%d): Attempting to send some data", conn->id, conn->ip, conn->port);
    knRBuff_peek(conn->out_buff, tmp, usage);
    ssize_t sends = sendto(server->fd, tmp, usage, KN_NOSIGNAL, (struct sockaddr *)&conn->addr, conn->addr_length);
    if (sends > 0) {
        knRBuff_pop(conn->out_buff, NULL, sends);
        knInfo(server->logger, "Client [%zu]: sent %zu bytes, remaining: %zu bytes.", conn->id, (size_t)sends, knRBuff_usage(conn->out_buff));
        
        if (knRBuff_isEmpty(conn->out_buff)) {
            if (server->onWrite)
                server->onWrite(conn);
        } else {
            ctx->packets_remaining = true;
        }
    } else {
        knError(server->logger, "Client [%zu]: Failed to send data.", conn->id);
        ctx->packets_remaining = true;
    }
}

int knServer_udpPolloutHook(
    knServer* server,
    size_t *idx KN_UNUSED
)
{
    knPolloutContext ctx = { .server = server, .packets_remaining = false };

    if (!server->on_udp.connections) {
        return KNEVTOK;
    }
    knMap_foreach(server->on_udp.connections, &__pollout, &ctx);
    if (!ctx.packets_remaining) {
        server->pool.pollfds[0].events = POLLIN;
    }
    return KNEVTOK;
}
