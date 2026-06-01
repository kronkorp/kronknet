/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init poolfd
*/
#include "kronknet/connection/connection.h"
#include "kronknet/server/pool/pool.h"
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>

int knPool_init(knPool *pool)
{
    if (!pool) {
        return -1;
    }
    pool->count = 0;
    pool->size = 1;
    pool->pollfds = calloc(pool->size, sizeof(struct pollfd));
    pool->conns = calloc(pool->size, sizeof(knConnection *));
    pool->conns[0] = NULL;
    if (!pool->pollfds) {
        return -1;
    }
    return 0;
}
