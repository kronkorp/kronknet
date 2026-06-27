/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Create knMap
*/
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "hashmap.h"

static int __knMap_addNode(
    knMapNode** bucket,
    void* value,
    uint64_t hash,
    knMapDeleter deleter,
    uint64_t key
)
{
    knMapNode* node = calloc(1, sizeof(knMapNode));

    if (!node) {
        return -1;
    }
    node->data = value;
    node->deleter = deleter;
    node->hash = hash;
    node->next = *bucket;
    node->prev = NULL;
    node->key = key;
    if (node->next) {
        node->next->prev = node;
    }
    *bucket = node;
    return 0;
}

static void __knMap_overwrite(
    knMapNode* node,
    void* value,
    knMapDeleter deleter
)
{
    if (node->deleter) {
        node->deleter(node->data);
    }
    node->data = value;
    node->deleter = deleter;
}

KN_API
int knMap_insert(
    knMap *map,
    uint64_t key,
    void *value,
    knMapDeleter deleter
)
{
    uint64_t hashed;
    uint64_t idx;

    if (!map || !map->hash || map->size == 0) {
        return -1;
    }
    hashed = map->hash(key);
    idx = hashed % map->size;
    for (knMapNode* tmp = map->raw[idx]; tmp; tmp = tmp->next) {
        if (tmp->key == key) {
            __knMap_overwrite(tmp, value, deleter);
            return 0;
        }
    }
    if (((double)map->inserted / (double)map->size) > 0.75) {
        if (knMap_reHash(map, map->size * 2) == -1) {
            return -1;
        }
    }
    if (__knMap_addNode(&map->raw[idx], value, hashed, deleter, key) == -1) {
        return -1;
    }
    ++map->inserted;
    return 0;
}
