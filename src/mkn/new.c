/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** New impl for MKN
*/
#include "kronknet/mkn/object.h"
#include "kronknet/mkn/new.h"
#include "kronknet/errdef.h"

mknObject *new(const mknClass *class, ...)
{
    va_list list;
    mknObject *new = NULL;

    if (!class) {
        KN_PANIC("Bad parameter");
    }
    va_start(list, class);
    new = va_new(class, &list);
    va_end(list);
    return new;
}

mknObject *va_new(const mknClass *class, va_list *ap)
{
    mknObject *new = NULL;

    if (!class || !ap) {
        KN_PANIC("Bad parameter");
    }
    new = malloc(class->__size__);
    if (!new) {
        KN_PANIC("Memory allocation error");
    }
    memcpy(new, class, class->__size__);
    if (class->__ctor__) {
        class->__ctor__(new, ap);
    }
    return new;
}

void delete(mknObject *ptr)
{
    if (!ptr) {
        KN_PANIC("Bad parameter");
    }
    if (((mknClass *)ptr)->__dtor__) {
        ((mknClass *)ptr)->__dtor__(ptr);
    }
    free(ptr);
}
