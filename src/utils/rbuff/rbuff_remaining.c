/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Get remaining space from the isildur ring buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>

size_t isdrbuff_remaining(const knRBuff *buff)
{
    if (!buff) {
        return 0;
    }
    return (buff->size - 1) - ((buff->writer - buff->reader) & buff->mask);
}
