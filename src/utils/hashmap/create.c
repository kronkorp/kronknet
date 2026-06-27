/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Create knMap
*/
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdlib.h>
#include "hashmap.h"

KN_API
knMap* knMap_create(
    knMapHash hashFunc,
    size_t length
)
{
    knMap *map = NULL;

    if (length < 1) {
        return NULL;
    }
    map = calloc(1, sizeof(knMap));
    if (!map) {
        return NULL;
    }
    map->hash = hashFunc;
    map->size = length;
    map->raw = calloc(map->size, sizeof(knMapNode *));
    if (!map->raw) {
        free(map);
        return NULL;
    }
    return map;
}
