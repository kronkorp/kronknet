/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Header file for KRONKNET, a simple ring-buffer library in C.
*/
#ifndef KRONKNET_HASHMAP_IMPL_H
    #define KRONKNET_HASHMAP_IMPL_H
    #include "kronknet/utils/hashmap/hashmap.h"
    #include <stddef.h>
    #include <stdint.h>

typedef struct kronknet_hashmap_node_s {

    uint64_t                        key;        //!< The unhashed key (ip << 32 | port)
    uint64_t                        hash;       //!< The hash (to overwring)
    void*                           data;       //!< The connection data ptr
    struct kronknet_hashmap_node_s* next;       //!< The next node
    struct kronknet_hashmap_node_s* prev;       //!< The previous node
    knMapDeleter                    deleter;    //!< How to clear this data

} knMapNode;

// NOTE: Using for UDP mode in kronknet
typedef struct kronknet_hashmap_s {

    size_t      inserted;  //!< The nbr of insterted data
    size_t      size;      //!< The size of the hashmap
    knMapNode** raw;       //!< The raw array of nodes
    knMapHash   hash;      //!< The hash function

} knMap;

int knMap_reHash(knMap* map, size_t newSize);

#endif /* KRONKNET_HASHMAP_IMPL_H */
