/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Create a KRONKNET buffer
*/
#include "rbuff.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "errno.h"

knRBuff *knRBuff_create(
    size_t size
)
{
    knRBuff *buff = calloc(1, sizeof(knRBuff));

    if (!buff) {
        errno = ENOMEM;
        return NULL;
    }
    if (knRBuff_init(buff, size) != 0) {
        free(buff);
        return NULL;
    }
    return buff;
}
