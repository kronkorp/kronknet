/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** reHash
*/
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "hashmap.h"

// NOTE: Call when load factor > 0.75
int knMap_reHash(
    knMap* map,
    size_t newSize
)
{
    knMapNode** newRaw = NULL;
    size_t newIndex;
    knMapNode *next, *current;

    if (!map || newSize == 0 || !map->hash)
        return -1;
    newRaw = calloc(newSize, sizeof(knMapNode *));
    if (!newRaw)
        return -1;
    for (size_t i = 0; i < map->size; ++i) {
        current = map->raw[i];
        while (current) {
            next = current->next;
            newIndex = (size_t)(current->hash % newSize);
            current->next = newRaw[newIndex];
            current->prev = NULL;
            if (newRaw[newIndex]) {
                newRaw[newIndex]->prev = current;
            }
            newRaw[newIndex] = current;
            current = next;
        }
    }
    free(map->raw);
    map->raw = newRaw;
    map->size = newSize;
    return 0;
}
