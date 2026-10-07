/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Get time in ms from a monotonic clock.
*/
#pragma once
#include "kronknet/macros/optimization.h"
#include <stdint.h>

typedef uint64_t timestamp;

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Get the time in ms from a monotonic clock
 *
 * @note    Only the difference between two of them means something
 *
 * @return  The time in ms, or 0 on error
 */
///////////////////////////////////////////////////////////////////////////////
KN_API KN_HOT timestamp monotonic(void);
