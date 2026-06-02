/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Push datas into isildur ring buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

ssize_t isdrbuff_push(knRBuff *buff, const uint8_t *src, size_t size)
{
    if (!buff || !src || size == 0) {
        errno = EINVAL;
        return -1;
    }
    if (size > isdrbuff_remaining(buff)) {
        errno = ENOBUFS;
        return -1;
    }
    size_t bytes_to_end = buff->size - buff->writer;
    if (size <= bytes_to_end) {
        memcpy(&buff->raw_buffer[buff->writer], src, size);
    } else {
        memcpy(&buff->raw_buffer[buff->writer], src, bytes_to_end);
        size_t reminder = size - bytes_to_end;
        memcpy(buff->raw_buffer, &src[bytes_to_end], reminder);
    }
    buff->writer = (buff->writer + size) & buff->mask;
    return size;
}
