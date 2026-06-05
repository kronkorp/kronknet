/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Server impl for MKN
*/
#include "kronknet/server/server.h"
#include "kronknet/callback/callback.h"
#include "kronknet/mkn/object.h"
#include <cstdint>
#include <stdarg.h>
#include <stdio.h>

typedef struct {

    mknClass base;
    knServer *server;

} mknServerClass;

static void mknServer_ctor(mknServerClass *thus, va_list *args)
{
    if (!thus || !args) {
        raise("Bad parameter");
    }
    thus->server = knServer_create(va_arg(*args, int));
    if (!thus->server) {
        raise("[mknServer]: Failed to initialize internal ressources");
    }
}

static void mknServer_dtor(mknServerClass *thus)
{
    if (thus) {
        knServer_destroy(thus->server);
    }
}

static const mknServerClass _description = {
    {
        .__size__ = sizeof(_description),
        .__name__ = "mknServer",
    },
    .server = NULL,
};

const mknClass *mknServer = (const mknClass *)&_description;
