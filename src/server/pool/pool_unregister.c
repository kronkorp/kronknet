/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Unregister a socket from the server's pool
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../connection/connection.h"

int knPool_unregister(
    knPool *pool,
    knSocket fd
)
{
    if (!pool || fd == KN_INVALID_SOCKET) {
        return KNEVTERR;
    }
    for (size_t i = 0; i < pool->count; ++i) {
        if (pool->conns[i] && pool->conns[i]->fd == fd) {
            return knPool_unregisterAtIndex(pool, i);
        }
    }
    return KNEVTERR;
}

int knPool_unregisterAtIndex(
    knPool *pool,
    size_t index
)
{
    if (!pool || index == (size_t)-1 || index >= pool->count) {
        return KNEVTERR;
    }
    if (pool->conns[index] && pool->conns[index]->fd != KN_INVALID_SOCKET) {
        knPoller_remove(pool->poller, pool->conns[index]->fd);
    }
    pool->conns[index] = pool->conns[pool->count - 1];
    pool->conns[pool->count - 1] = NULL;
    pool->count--;
    return KNEVTOK;
}
