/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init a KRONKNET buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <stddef.h>
#include <stdint.h>

bool knRBuff_isEmpty(const knRBuff *buff)
{
    return knRBuff_usage(buff) == 0;
}
