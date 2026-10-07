/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Init poolfd
*/
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "pool.h"
#include <stdlib.h>

int knPool_init(
    knPool *pool
)
{
    if (!pool) {
        return KNEVTARGS;
    }
    pool->count = 0;
    pool->size = 1;
    pool->conns = calloc(pool->size, sizeof(knConnection *));
    if (!pool->conns) {
        return KNEVTMEM;
    }
    pool->conns[0] = NULL;
    pool->poller = knPoller_create();
    if (!pool->poller) {
        free(pool->conns);
        pool->conns = NULL;
        return KNEVTNET;
    }
    return KNEVTOK;
}
