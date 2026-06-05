/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Server def for MKN
*/
#ifndef MKRONKNET_SERVER_H
    #define MKRONKNET_SERVER_H
    #include "kronknet/mkn/object.h"
    #include <stddef.h>

typedef struct mknServerContainer_s mknServerContainer;

typedef int (*mkn_run_t)(mknServerContainer *);
typedef int (*mkn_runOnce_t)(mknServerContainer *, ssize_t);
typedef int (*mkn_setCb_t)(mknServerContainer *, int (*)());
typedef int (*mkn_getInt_t)(mknServerContainer *);
typedef int (*mkn_set_t)(mknServerContainer *, ...);

struct mknServerContainer_s {

    mknClass      base;
    mkn_run_t     __run__;
    mkn_runOnce_t __runOnce__;
    mkn_setCb_t   __onRead__;
    mkn_setCb_t   __onWrite__;
    mkn_setCb_t   __onConnect__;
    mkn_setCb_t   __onDisconnect__;
    mkn_getInt_t  __port__;
    mkn_set_t     __log__;
    mkn_set_t     __data__;

};

#define run(o)              (((mknServerContainer *)o)->__run__(o))
#define runOnce(o, t)       (((mknServerContainer *)o)->__runOnce__(o, t))
#define onRead(o, c)        (((mknServerContainer *)o)->__onRead__(o, c))
#define onWrite(o, c)       (((mknServerContainer *)o)->__onWrite__(o, c))
#define onConnect(o, c)     (((mknServerContainer *)o)->__onConnect__(o, c))
#define onDisconnect(o, c)  (((mknServerContainer *)o)->__onDisconnect__(o, c))
#define port(o)             (((mknServerContainer *)o)->__port__(o))
#define setLogging(o, ...)  (((mknServerContainer *)o)->__log__(o, __VA_ARGS__))
#define setData(o, ...)     (((mknServerContainer *)o)->__data__(o, __VA_ARGS__))

extern const mknClass *mknServer;

#endif /* MKRONKNET_SERVER_H */
