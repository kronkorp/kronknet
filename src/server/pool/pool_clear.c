/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Clear pool
*/
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/poll.h>
#include "../../connection/connection.h"

void knPool_clear(
    knPool *pool
)
{
    if (!pool) {
        return;
    }
    if (pool->pollfds) {
        free(pool->pollfds);
    }
    if (pool->conns) {
        for (size_t i = 0; i < pool->count; ++i){
            knConnection_destroy(pool->conns[i]);
        }
        free(pool->conns);
    }
    pool->count = 0;
    pool->size = 0;
}
