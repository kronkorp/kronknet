/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Register an fd for the server's pool
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/server/pool/pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

static int __knPool_ensureCapacity(knPool *pool, size_t size)
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

int knPool_registerFd(knPool *pool, int fd, int events)
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
    pool->count = new_count;
    return KNEVTOK;
}

// FIXME: ensure capacity if needed.
int knPool_registerConnection(knPool *pool, knConnection *connection)
{
    // size_t newCount = 0;

    if (!pool) {
        return KNEVTARGS;
    }
    // newCount = pool->count + 1;
    // if (__knPool_ensureCapacity(pool, newCount) == -1) {
    //     return -1;
    // }
    pool->conns[pool->count - 1] = connection;
    // pool->count = newCount;
    return KNEVTOK;
}
