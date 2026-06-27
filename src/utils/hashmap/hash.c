/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Thomas Wang hash algorithm: https://gist.github.com/badboy/6267743
*/
#include <stdint.h>

uint64_t knMap_basicHash(
    uint64_t key
)
{
    key = (~key) + (key << 21); 
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}
