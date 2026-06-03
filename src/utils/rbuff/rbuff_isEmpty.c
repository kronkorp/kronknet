/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Init a isildur buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include <stdint.h>

bool knRBuff_isEmpty(const knRBuff *buff)
{
    return knRBuff_usage(buff) == 0;
}
