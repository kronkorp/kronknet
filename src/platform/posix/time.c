/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Monotonic clock on POSIX.
*/
#include "kronknet/utils/monotonic.h"
#include <time.h>

KN_API
KN_HOT
timestamp monotonic(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        return 0;
    }
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
