/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Create a isildur buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "errno.h"

knRBuff *isdrbuff_create(size_t size)
{
    knRBuff *buff = calloc(1, sizeof(knRBuff));

    if (!buff) {
        errno = ENOMEM;
        return NULL;
    }
    if (isdrbuff_init(buff, size) != 0) {
        free(buff);
        return NULL;
    }
    return buff;
}
