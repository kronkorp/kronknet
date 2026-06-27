/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Search in knMap
*/
#include "kronknet/macros/optimization.h"
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include "hashmap.h"

KN_API
void* knMap_search(
    const knMap* map,
    uint64_t key
)
{
    uint64_t hashed;

    if (!map || map->size == 0 || !map->hash) {
        return NULL;
    }
    hashed = map->hash(key);
    for (knMapNode* tmp = map->raw[hashed % map->size]; tmp; tmp = tmp->next) {
        if (tmp->key == key) {
            return tmp->data;
        }
    }
    return NULL;
}
