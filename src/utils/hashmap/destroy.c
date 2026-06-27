/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Destroy hashmap
*/
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdlib.h>
#include "hashmap.h"

static void __knMap_destroyBucket(
    knMapNode* node
)
{
    if (!node) {
        return;
    }
    if (node->next) {
        __knMap_destroyBucket(node->next);
    }
    if (node->deleter) {
        node->deleter(node->data);
    }
    free(node);
}

KN_API
void knMap_destroy(
    knMap *map
)
{
    if (!map) {
        return;
    }
    for (size_t i = 0; i < map->size; ++i) {
        __knMap_destroyBucket(map->raw[i]);
    }
    free(map->raw);
    free(map);
}
