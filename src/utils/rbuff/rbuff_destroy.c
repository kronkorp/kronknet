/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy an KRONKNET buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include "stdlib.h"
#include "rbuff.h"

void knRBuff_destroy(knRBuff *buff)
{
    if (!buff) {
        return;
    }
    knRBuff_clean(buff);
    free(buff);
}
