/*
** EPITECH PROJECT, 2026
** ISILDUR
** File description:
** Destroy an isildur buffer
*/
#include "kronknet/utils/rbuff/rbuff.h"
#include <errno.h>
#include "stdlib.h"

void isdrbuff_destroy(knRBuff *buff)
{
    if (!buff) {
        return;
    }
    isdrbuff_clean(buff);
    free(buff);
}
