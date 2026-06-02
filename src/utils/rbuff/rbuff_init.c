/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Init a isildur buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "errno.h"

int isdrbuff_init(knRBuff *buff, size_t size)
{
    if (!buff || size == 0 || ((size & (size - 1)) != 0)) {
        errno = EINVAL;
        return -1;
    }
    buff->raw_buffer = calloc(size, sizeof(uint8_t));
    if (!buff->raw_buffer) {
        errno = ENOMEM;
        return -1;
    }
    buff->size = size;
    buff->mask = size - 1;
    buff->reader = 0;
    buff->reader = 0;
    return 0;
}
