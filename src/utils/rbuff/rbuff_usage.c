/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get using space from the KRONKNET ring buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include "rbuff.h"

size_t knRBuff_usage(
    const knRBuff *buff
)
{
    if (!buff) {
        return 0;
    }
    return (buff->writer - buff->reader) & buff->mask;
}
