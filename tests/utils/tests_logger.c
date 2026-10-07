/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Tests of the logger and of the log level of the server and the client
*/
#include <kronklab/kronklab.h>
#include "kronknet/client/client.h"
#include "kronknet/macros/types.h"
#include "kronknet/server/server.h"
#include "../../src/utils/logger/logger.h"
#include <stdio.h>
#include <string.h>

//! Everything written to [out] since it was opened
static void read_all(FILE *out, char *buffer, size_t size)
{
    size_t len;

    rewind(out);
    len = fread(buffer, 1, size - 1, out);
    buffer[len] = '\0';
}

Test(logger, level_filters_what_is_below)
{
    FILE *out = tmpfile();
    knLoggerData logger = { .out = out, .log_level = knLogWarn };
    char written[256];

    AssertNotNull(out, "A temporary file should open");
    knInfo(logger, "info %d", 1);
    knWarn(logger, "warn %d", 2);
    knError(logger, "error %d", 3);
    read_all(out, written, sizeof(written));
    AssertStrEq(written, "WARN: warn 2\nERROR: error 3\n", "Only what is at the level or above should be written");
    fclose(out);
}

Test(logger, level_none_writes_nothing)
{
    FILE *out = tmpfile();
    knLoggerData logger = { .out = out, .log_level = knLogNone };
    char written[256];

    AssertNotNull(out, "A temporary file should open");
    knFatal(logger, "fatal");
    read_all(out, written, sizeof(written));
    AssertStrEq(written, "", "knLogNone should write nothing, not even a fatal");
    fclose(out);
}

Test(logger, server_level)
{
    knServer *server = knServer_create(43101, knTCP);

    AssertNotNull(server, "Server creation should succeed");
    AssertEq(knServer_getLogLevel(server), knLogNone, "A server logs nothing until asked to");
    knServer_setLogLevel(server, knLogWarn);
    AssertEq(knServer_getLogLevel(server), knLogWarn, "The level should be the one that was set");
    knServer_destroy(server);
}

Test(logger, client_level)
{
    knClient *client = knClient_create(knTCP);

    AssertNotNull(client, "Client creation should succeed");
    AssertEq(knClient_getLogLevel(client), knLogNone, "A client logs nothing until asked to");
    knClient_setLogLevel(client, knLogError);
    AssertEq(knClient_getLogLevel(client), knLogError, "The level should be the one that was set");
    knClient_destroy(client);
}
