/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Wait for events on many sockets, the same way on every platform.
*/
#ifndef KRONKNET_PLATFORM_POLLER_H
    #define KRONKNET_PLATFORM_POLLER_H
    #include "socket.h"
    #include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   A set of sockets to wait on: epoll on Linux, wepoll on Windows
 *          (src/platform/<platform>/poller.c)
 *
 * @note    Level-triggered: a socket is reported as long as it is ready
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_poller_s knPoller;

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   What knPoller_wait reports for one socket
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_poll_event_s {

    uint32_t events;  //!< What happened (KN_POLL*)
    void    *ptr;     //!< The pointer given with the socket

} knPollEvent;

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Create an empty poller
 *
 * @return  The poller, or NULL
 */
///////////////////////////////////////////////////////////////////////////////
knPoller *knPoller_create(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Destroy a poller (not the sockets in it)
 */
///////////////////////////////////////////////////////////////////////////////
void knPoller_destroy(knPoller *poller);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Add a socket, or change what to wait for on it
 *
 * @param   poller The poller
 * @param   fd     The socket
 * @param   ptr    What knPoller_wait gives back with its events
 * @param   events What to wait for (KN_POLLIN, KN_POLLOUT)
 *
 * @return  KNEVTOK, or KNEVTNET
 */
///////////////////////////////////////////////////////////////////////////////
int knPoller_add(knPoller *poller, knSocket fd, void *ptr, uint32_t events);
int knPoller_modify(knPoller *poller, knSocket fd, void *ptr, uint32_t events);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Remove a socket (before it is closed)
 *
 * @return  KNEVTOK, or KNEVTNET
 */
///////////////////////////////////////////////////////////////////////////////
int knPoller_remove(knPoller *poller, knSocket fd);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Wait until sockets are ready
 *
 * @param   poller    The poller
 * @param   events    Where to write what happened
 * @param   max       How many events [events] can take
 * @param   timeoutMs How long to wait at most (-1 for no limit)
 *
 * @return  The number of events (0 on timeout), or -1 on error
 */
///////////////////////////////////////////////////////////////////////////////
int knPoller_wait(knPoller *poller, knPollEvent *events, int max, int timeoutMs);

#endif /* KRONKNET_PLATFORM_POLLER_H */
