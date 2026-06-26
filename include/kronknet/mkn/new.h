/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Object construction helpers for the MKN layer.
*/
#ifndef MKRONKNET_NEW_H
    #define MKRONKNET_NEW_H
    #include <stdarg.h>
    #include "kronknet/mkn/object.h"

mknObject *new(const mknClass *class, ...);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Construct a new object instance from a va_list.
 */
///////////////////////////////////////////////////////////////////////////////
mknObject *va_new(const mknClass *class, va_list *ap);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destroy an object instance.
 */
///////////////////////////////////////////////////////////////////////////////
void delete(mknObject *ptr);

#endif /* MKRONKNET_NEW_H */
