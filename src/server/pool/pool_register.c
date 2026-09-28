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
#include <sys/epoll.h>
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
    struct epoll_event ev = {
        .events = events,
        .data.ptr = conn,
    };

    if (!pool || fd == -1) {
        return KNEVTARGS;
    }
    new_count = pool->count + 1;
    int err = __knPool_ensureCapacity(pool, new_count);
    if (err != KNEVTOK) {
        return err;
    }
    if (epoll_ctl(pool->epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        return KNEVTNET;
    }
    pool->conns[pool->count] = conn;
    if (conn) {
        conn->epollfd = pool->epollfd;
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
    struct epoll_event ev = {
        .events = events,
        .data.ptr = conn,
    };

    if (!pool || fd == -1) {
        return KNEVTARGS;
    }
    if (epoll_ctl(pool->epollfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}
