/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Core object model used by the MKN layer.
*/
#ifndef MKRONKNET_OBJECT_H
    #define MKRONKNET_OBJECT_H
    #include <sys/types.h>
    #include <stdarg.h>
    #include <stdbool.h>
    #include <string.h>
    #include "kronknet/macros/errdef.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Opaque base type for all MKN objects.
 */
///////////////////////////////////////////////////////////////////////////////
typedef void mknObject;
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Constructor signature used by MKN classes.
 */
///////////////////////////////////////////////////////////////////////////////
typedef void (*ctor_t)(mknObject *this, va_list *args);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destructor signature used by MKN classes.
 */
///////////////////////////////////////////////////////////////////////////////
typedef void (*dtor_t)(mknObject *this);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief String conversion signature used by MKN classes.
 */
///////////////////////////////////////////////////////////////////////////////
typedef char *(*to_string_t)(mknObject *this);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Binary operator signature used by MKN classes.
 */
///////////////////////////////////////////////////////////////////////////////
typedef mknObject *(*binary_operator_t)(const mknObject *this, const mknObject *other);
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Binary comparator signature used by MKN classes.
 */
///////////////////////////////////////////////////////////////////////////////
typedef bool (*binary_comparator_t)(const mknObject *this, const mknObject *other);

///////////////////////////////////////////////////////////////////////////////
typedef struct mkronknet_class_s {

    const size_t        __size__;
    const char*         __name__;
    ctor_t              __ctor__;
    dtor_t              __dtor__;
    to_string_t         __str__;
    binary_operator_t   __add__;
    binary_operator_t   __sub__;
    binary_operator_t   __mul__;
    binary_operator_t   __div__;
    binary_comparator_t __eq__;
    binary_comparator_t __gt__;
    binary_comparator_t __lt__;

} mknClass;

#define str(o)                  (((mknClass *)o)->__str__ != NULL ? ((mknClass *)o)->__str__(o) : strdup(((mknClass *)o)->__name__))
#define addition(a, b)          ((mknClass *)a)->__add__(a, b)
#define subtraction(a, b)       ((mknClass *)a)->__sub__(a, b)
#define multiplication(a, b)    ((mknClass *)a)->__mul__(a, b)
#define division(a, b)          ((mknClass *)a)->__div__(a, b)
#define eq(a, b)                ((mknClass *)a)->__eq__(a, b)
#define gt(a, b)                ((mknClass *)a)->__gt__(a, b)
#define lt(a, b)                ((mknClass *)a)->__lt__(a, b)

#endif /* MKRONKNET_OBJECT_H */
