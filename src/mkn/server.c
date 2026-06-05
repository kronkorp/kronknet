/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Server impl for MKN
*/
#include "kronknet/server/server.h"
#include "kronknet/callback/callback.h"
#include "kronknet/errdef.h"
#include "kronknet/mkn/object.h"
#include "kronknet/mkn/server.h"
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {

    mknServerContainer base;
    knServer*          _server;

} mknServerClass;

static void mknServer_ctor(mknServerClass *thus, va_list *args)
{
    if (!thus || !args) {
        KN_PANIC("Bad parameter");
    }
    thus->_server = knServer_create(va_arg(*args, int));
    if (!thus->_server) {
        KN_PANIC("[mknServer]: Failed to initialize internal ressources");
    }
}

static void mknServer_dtor(mknServerClass *thus)
{
    if (thus) {
        knServer_destroy(thus->_server);
    }
}

static char *mknServer_toString(mknServerClass *thus)
{
    size_t size = 0;
    char *buffer = 0;

    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    size = snprintf(NULL, 0, "<%s (localhost:%d)>", thus->base.base.__name__, port(&thus->base));
    buffer = malloc(sizeof(char) * (size + 1));
    if (!buffer) {
        KN_PANIC("Memory allocation failed");
    }
    snprintf(buffer, size + 1, "<%s (localhost:%d)>", thus->base.base.__name__, port(&thus->base));
    return buffer;
}

static int mknServer_run(mknServerClass *thus)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_run(thus->_server);
}

static int mknServer_runOnce(mknServerClass *thus, ssize_t timeout)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_runOnce(thus->_server, timeout);
}

static int mknServer_onReadCb(mknServerClass *thus, knReadCb cb)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_onReadCallback(thus->_server, cb);
}

static int mknServer_onWriteCb(mknServerClass *thus, knEventCb cb)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_onWriteCallback(thus->_server, cb);
}

static int mknServer_onConnectCb(mknServerClass *thus, knConnectionCb cb)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_onConnectionCallback(thus->_server, cb);
}

static int mknServer_onDisconnectCb(mknServerClass *thus, knConnectionCb cb)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return knServer_onDisconnectionCallback(thus->_server, cb);
}

static int mknServer_getPort(mknServerClass *thus)
{
    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    return ntohs(thus->_server->addr.sin_port);
}

static int mknServer_setLogging(mknServerClass *thus, ...)
{
    va_list l;

    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    va_start(l, thus);
    bool c = va_arg(l, int);
    va_end(l);
    knServer_setLogging(thus->_server, c);
    return KNEVTOK;
}

static int mknServer_setData(mknServerClass *thus, ...)
{
    va_list l;

    if (!thus) {
        KN_PANIC("Bad parameter");
    }
    va_start(l, thus);
    knServer_setData(thus->_server, va_arg(l, void *));
    va_end(l);
    return KNEVTOK;
}

static const mknServerClass _description = {
    {
        {
            .__size__ = sizeof(_description),
            .__name__ = "mknServer",
            .__ctor__ = (ctor_t)&mknServer_ctor,
            .__dtor__ = (dtor_t)&mknServer_dtor,
            .__str__  = (to_string_t)&mknServer_toString,
            .__add__  = NULL,
            .__sub__  = NULL,
            .__mul__  = NULL,
            .__div__  = NULL,
            .__eq__   = NULL,
            .__gt__   = NULL,
            .__lt__   = NULL,
        },
        .__run__          = (mkn_run_t)&mknServer_run,
        .__runOnce__      = (mkn_runOnce_t)&mknServer_runOnce,
        .__onConnect__    = (mkn_setCb_t)&mknServer_onConnectCb,
        .__onRead__       = (mkn_setCb_t)&mknServer_onReadCb,
        .__onWrite__      = (mkn_setCb_t)&mknServer_onWriteCb,
        .__onDisconnect__ = (mkn_setCb_t)&mknServer_onDisconnectCb,
        .__port__         = (mkn_getInt_t)&mknServer_getPort,
        .__log__          = (mkn_set_t)&mknServer_setLogging,
        .__data__         = (mkn_set_t)&mknServer_setData,
    },
    ._server = NULL,
};

const mknClass *mknServer = (const mknClass *)&_description;
