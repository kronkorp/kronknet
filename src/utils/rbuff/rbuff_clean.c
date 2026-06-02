/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Clean an isildur buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include "stdlib.h"

void isdrbuff_clean(knRBuff *buff)
{
    if (buff->raw_buffer) {
        free(buff->raw_buffer);
    }
}
