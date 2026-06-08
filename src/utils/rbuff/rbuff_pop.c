/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Pop datas from KRONKNET ring buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

ssize_t knRBuff_pop(knRBuff *buff, uint8_t *dest, size_t size)
{
    if (!buff || size == 0) {
        errno = EINVAL;
        return -1;
    }
    if (size > knRBuff_usage(buff)) {
        errno = EAGAIN;
        return -1;
    }
    if (dest != NULL) {
        size_t bytes_to_end = buff->size - buff->reader;
        if (size <= bytes_to_end) {
            memcpy(dest, &buff->raw_buffer[buff->reader], size);
        } else {
            memcpy(dest, &buff->raw_buffer[buff->reader], bytes_to_end);
            size_t reminder = size - bytes_to_end;
            memcpy(&dest[bytes_to_end], buff->raw_buffer, reminder);
        }
    }
    buff->reader = (buff->reader + size) & buff->mask;
    return size;
}
