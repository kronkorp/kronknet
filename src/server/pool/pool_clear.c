/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Clear pool
*/
#include "kronknet/server/pool/pool.h"
#include <stdlib.h>
#include <sys/poll.h>

void knPool_clear(knPool *pool)
{
    if (!pool) {
        return;
    }
    if (pool->pollfds) {
        free(pool->pollfds);
    }
    if (pool->conns) {
        free(pool->conns);
    }
    pool->count = 0;
    pool->size = 0;
}
