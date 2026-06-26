/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Clean an KRONKNET buffer
*/
#include "rbuff.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "stdlib.h"

void knRBuff_clean(
    knRBuff *buff
)
{
    if (buff->raw_buffer) {
        free(buff->raw_buffer);
    }
}
