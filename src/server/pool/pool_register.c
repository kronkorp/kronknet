/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Register an fd for the server's pool
*/
#include "kronknet/connection/connection.h"
#include "kronknet/server/pool/pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

static int __knPool_ensureCapacity(knPool *pool, size_t size)
{
    if (!pool) {
        return -1;
    }
    while (pool->size < size) {
        pool->pollfds = realloc(pool->pollfds, sizeof(struct pollfd) * pool->size * 2);
        if (pool->pollfds == NULL)
            return -1;
        memset(&pool->pollfds[pool->size], 0, sizeof(struct pollfd) * pool->size);
        pool->conns = realloc(pool->conns, sizeof(knConnection *) * pool->size * 2);
        if (pool->conns == NULL)
            return -1;
        memset(&pool->conns[pool->size], 0, sizeof(knConnection *) * pool->size);
        pool->size *= 2;
    }
    return 0;
}

int knPool_registerFd(knPool *pool, int fd, int events)
{
    size_t new_count = 0;

    if (!pool || fd == -1) {
        return -1;
    }
    new_count = pool->count + 1;
    if (__knPool_ensureCapacity(pool, new_count) == -1) {
        return -1;
    }
    pool->pollfds[pool->count].fd = fd;
    pool->pollfds[pool->count].events = events;
    pool->count = new_count;
    return 0;
}

int knPool_registerConnection(knPool *pool, knConnection *connection)
{
    size_t newCount = 0;

    if (!pool) {
        return -1;
    }
    newCount = pool->count + 1;
    if (__knPool_ensureCapacity(pool, newCount) == -1) {
        return -1;
    }
    pool->conns[pool->count] = connection;
    pool->count = newCount;
    return 0;
}
