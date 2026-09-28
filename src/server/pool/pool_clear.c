/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Clear pool
*/
#include "pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../connection/connection.h"

void knPool_clear(
    knPool *pool
)
{
    if (!pool) {
        return;
    }
    if (pool->conns) {
        for (size_t i = 0; i < pool->count; ++i){
            knConnection_destroy(pool->conns[i]);
        }
        free(pool->conns);
        pool->conns = NULL;
    }
    if (pool->epollfd != -1) {
        close(pool->epollfd);
        pool->epollfd = -1;
    }
    pool->count = 0;
    pool->size = 0;
}
