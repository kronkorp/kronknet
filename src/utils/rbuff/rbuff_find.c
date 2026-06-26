/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** knRBuff_find impl
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <string.h>
#include <stddef.h>
#include "rbuff.h"

ssize_t knRBuff_find(
    const knRBuff *buff,
    const uint8_t *data,
    size_t size
)
{
    if (!buff || !data || size == 0) {
        return -1;
    }
    size_t total = knRBuff_usage(buff);
    if (size > total) {
        return -1;
    }
    for (size_t i = 0; i <= total - size; ++i) {
        size_t idx = (buff->reader + i) & buff->mask;
        size_t rem = buff->size - idx;
        if (size <= rem) {
            if (memcmp(&buff->raw_buffer[idx], data, size) == 0) {
                return (ssize_t)i;
            }
        } else {
            size_t nsize = rem;
            size_t reminder = size - nsize;
            if (memcmp(&buff->raw_buffer[idx], data, nsize) == 0 &&
                memcmp(&buff->raw_buffer[0], &data[nsize], reminder) == 0) {
                return (ssize_t)i;
            }
        }
    }
    return -1;
}