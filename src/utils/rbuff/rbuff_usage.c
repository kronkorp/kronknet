/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Get using space from the isildur ring buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>

size_t knRBuff_usage(const knRBuff *buff)
{
    if (!buff) {
        return 0;
    }
    return (buff->writer - buff->reader) & buff->mask;
}
