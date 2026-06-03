/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init poolfd
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/server/pool/pool.h"
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>

int knPool_init(knPool *pool)
{
    if (!pool) {
        return KNEVTARGS;
    }
    pool->count = 0;
    pool->size = 1;
    pool->pollfds = calloc(pool->size, sizeof(struct pollfd));
    if (!pool->pollfds) {
        return KNEVTMEM;
    }
    pool->conns = calloc(pool->size, sizeof(knConnection *));
    if (!pool->conns) {
        free(pool->pollfds);
        return KNEVTMEM;
    }
    pool->conns[0] = NULL;
    return KNEVTOK;
}
