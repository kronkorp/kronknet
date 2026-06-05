/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** New def for MKN
*/
#ifndef MKRONKNET_NEW_H
    #define MKRONKNET_NEW_H
    #include <stdarg.h>
    #include "kronknet/mkn/object.h"

mknObject *new(const mknClass *class, ...);
mknObject *va_new(const mknClass *class, va_list *ap);
void delete(mknObject *ptr);

#endif /* MKRONKNET_NEW_H */
