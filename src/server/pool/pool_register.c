/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Register an fd for the server's pool
*/
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../connection/connection.h"

static int __knPool_ensureCapacity(
    knPool *pool,
    size_t size
)
{
    if (!pool) {
        return KNEVTARGS;
    }
    while (pool->size < size) {
        pool->conns = realloc(pool->conns, sizeof(knConnection *) * pool->size * 2);
        if (pool->conns == NULL)
            return KNEVTMEM;
        memset(&pool->conns[pool->size], 0, sizeof(knConnection *) * pool->size);
        pool->size *= 2;
    }
    return KNEVTOK;
}

int knPool_registerFd(
    knPool *pool,
    knSocket fd,
    knConnection *conn,
    uint32_t events
)
{
    size_t new_count = 0;

    if (!pool || fd == KN_INVALID_SOCKET) {
        return KNEVTARGS;
    }
    new_count = pool->count + 1;
    int err = __knPool_ensureCapacity(pool, new_count);
    if (err != KNEVTOK) {
        return err;
    }
    if (knPoller_add(pool->poller, fd, conn, events) != KNEVTOK) {
        return KNEVTNET;
    }
    pool->conns[pool->count] = conn;
    if (conn) {
        conn->poller = pool->poller;
    }
    pool->count = new_count;
    return KNEVTOK;
}

int knPool_modifyFd(
    knPool *pool,
    knSocket fd,
    knConnection *conn,
    uint32_t events
)
{
    if (!pool || fd == KN_INVALID_SOCKET) {
        return KNEVTARGS;
    }
    if (knPoller_modify(pool->poller, fd, conn, events) != KNEVTOK) {
        return KNEVTNET;
    }
    return KNEVTOK;
}
