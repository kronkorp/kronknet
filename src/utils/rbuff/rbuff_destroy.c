/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy an KRONKNET buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <errno.h>
#include "stdlib.h"

void knRBuff_destroy(knRBuff *buff)
{
    if (!buff) {
        return;
    }
    knRBuff_clean(buff);
    free(buff);
}
