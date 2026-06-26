/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Register an fd for the server's pool
*/
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
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
        pool->pollfds = realloc(pool->pollfds, sizeof(struct pollfd) * pool->size * 2);
        if (pool->pollfds == NULL)
            return KNEVTMEM;
        memset(&pool->pollfds[pool->size], 0, sizeof(struct pollfd) * pool->size);
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
    int fd,
    knConnection *conn,
    int events
)
{
    size_t new_count = 0;

    if (!pool || fd == -1) {
        return KNEVTARGS;
    }
    new_count = pool->count + 1;
    int err = __knPool_ensureCapacity(pool, new_count);
    if (err != KNEVTOK) {
        return err;
    }
    pool->pollfds[pool->count].fd = fd;
    pool->pollfds[pool->count].events = events;
    pool->conns[pool->count - 1] = conn;
    if (conn) {
        if (conn) {
            conn->evtptr = &pool->pollfds[pool->count - 1].events;
        }
    }
    pool->count = new_count;
    return KNEVTOK;
}
