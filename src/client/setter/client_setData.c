/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** et client data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"

void knClient_setData(knClient *client, void *data)
{
    if (!client) {
        return;
    }
    client->data = data;
}
