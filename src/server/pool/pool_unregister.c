/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Unregister an fd for the server's epoll
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "../../connection/connection.h"

int knPool_unregister(
    knPool *pool,
    knSocket fd
)
{
    if (!pool || fd == -1) {
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
    if (!pool || index == -1UL || index >= pool->count) {
        return KNEVTERR;
    }
    if (pool->conns[index] && pool->conns[index]->fd != -1) {
        epoll_ctl(pool->epollfd, EPOLL_CTL_DEL, pool->conns[index]->fd, NULL);
    }
    pool->conns[index] = pool->conns[pool->count - 1];
    pool->conns[pool->count - 1] = NULL;
    pool->count--;
    return KNEVTOK;
}
