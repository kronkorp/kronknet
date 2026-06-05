/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Errors and other defs
*/
#ifndef KRONKNET_ERRDEF_H
    #define KRONKNET_ERRDEF_H
    #include <stdlib.h>
    #include <stdio.h>

    #define KNEVTOK     0
    #define KNEVTERR   -1
    #define KNEVTMEM   -2
    #define KNEVTKICK  -4
    #define KNEVTNET   -8
    #define KNEVTARGS -16

    #define KN_PANIC(msg) \
    do { \
        fprintf(stderr, "%s:%d: FATAL ERROR: %s\n", __FILE__, __LINE__, msg); \
        abort(); \
    } while (0)

#endif /* KRONKNET_ERRDEF_H */
