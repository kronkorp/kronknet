/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Monotonic clock on Windows.
*/
#include "kronknet/utils/monotonic.h"
#include <windows.h>

KN_API
KN_HOT
timestamp monotonic(void)
{
    return (timestamp)GetTickCount64();
}
