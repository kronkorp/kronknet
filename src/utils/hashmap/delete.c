/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Delete data into knMap
*/
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include "hashmap.h"

static void __knMapNode_delete(
    knMapNode** bucket,
    knMapNode* node
)
{
    if (!node->prev) {
        *bucket = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    free(node);
}

KN_API
int knMap_delete(
    knMap *map,
    uint64_t key
)
{
    uint64_t hashed;
    uint64_t idx;

    if (!map || map->size == 0 || !map->hash) {
        return -1;
    }
    hashed = map->hash(key);
    idx = hashed % map->size;
    for (knMapNode* tmp = map->raw[idx]; tmp; tmp = tmp->next) {
        if (tmp->key == key) {
            __knMapNode_delete(&map->raw[idx], tmp);
            return 0;
        }
    }
    return -1;
}
