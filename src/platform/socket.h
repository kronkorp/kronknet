/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Sockets, the same way on every platform.
*/
#ifndef KRONKNET_PLATFORM_SOCKET_H
    #define KRONKNET_PLATFORM_SOCKET_H
    #include "kronknet/macros/types.h"
    #include <stddef.h>
    #include <stdint.h>

    // NOTE: The one place that knows where each platform keeps its socket API.
    //       What works differently is behind the knSocket_* functions below,
    //       written once per platform in src/platform/<platform>/ (CMake builds
    //       the one of the target)
    #ifdef _WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
    #else
        #include <arpa/inet.h>
        #include <netinet/in.h>
        #include <sys/socket.h>
    #endif /* _WIN32 */

    #define KN_POLLIN  0x01  //!< Readable (or a connection to accept)
    #define KN_POLLOUT 0x02  //!< Writable
    #define KN_POLLERR 0x04  //!< Error (reported even when not asked for)
    #define KN_POLLHUP 0x08  //!< Hung up (reported even when not asked for)

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Get the network stack ready (WSAStartup on Windows)
 *
 * @note    Each successful call needs its knSocket_cleanup
 *
 * @return  KNEVTOK, or KNEVTNET
 */
///////////////////////////////////////////////////////////////////////////////
int knSocket_startup(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Release what knSocket_startup took
 */
///////////////////////////////////////////////////////////////////////////////
void knSocket_cleanup(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Open a non-blocking IPv4 socket
 *
 * @param   type The type of the socket (SOCK_STREAM, SOCK_DGRAM)
 *
 * @return  The socket, or KN_INVALID_SOCKET
 */
///////////////////////////////////////////////////////////////////////////////
knSocket knSocket_open(int type);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Accept a connection, as a non-blocking socket
 *
 * @param   listener The listening socket
 * @param   addr     Where to write the address of the peer
 *
 * @return  The socket, or KN_INVALID_SOCKET
 */
///////////////////////////////////////////////////////////////////////////////
knSocket knSocket_accept(knSocket listener, struct sockaddr_in *addr);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Close a socket (nothing to do for KN_INVALID_SOCKET)
 */
///////////////////////////////////////////////////////////////////////////////
void knSocket_close(knSocket fd);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Let a server bind its port again right after it stopped, while
 *          its old connections are still in TIME_WAIT
 *
 * @return  KNEVTOK, or KNEVTNET
 */
///////////////////////////////////////////////////////////////////////////////
int knSocket_setReuseAddr(knSocket fd);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Do not fail a UDP server socket when a peer is gone
 *
 * @note    A datagram sent to a port nobody listens on is answered by an ICMP
 *          port unreachable. Linux reports nothing on a socket that is not
 *          connected; Windows fails its next recvfrom() (WSAECONNRESET)
 *
 * @return  KNEVTOK, or KNEVTNET
 */
///////////////////////////////////////////////////////////////////////////////
int knSocket_ignorePortUnreachable(knSocket fd);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   send() / recv() / sendto() / recvfrom(), the way POSIX does them
 *
 * @note    A peer that is gone makes them fail, never raises SIGPIPE.
 *          A datagram bigger than the buffer is cut to fit it
 *
 * @return  The number of bytes, or -1 (see knSocket_wouldBlock)
 */
///////////////////////////////////////////////////////////////////////////////
ssize_t knSocket_send(knSocket fd, const void *data, size_t size);
ssize_t knSocket_recv(knSocket fd, void *buff, size_t size);
ssize_t knSocket_sendTo(knSocket fd, const void *data, size_t size, const struct sockaddr_in *addr);
ssize_t knSocket_recvFrom(knSocket fd, void *buff, size_t size, struct sockaddr_in *addr);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Did the last socket call fail only because it would have blocked?
 */
///////////////////////////////////////////////////////////////////////////////
knBool knSocket_wouldBlock(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Did the last connect() fail only because it goes on in the background?
 */
///////////////////////////////////////////////////////////////////////////////
knBool knSocket_inProgress(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Wait for something to happen on one socket
 *
 * @param   fd        The socket
 * @param   events    What to wait for (KN_POLLIN, KN_POLLOUT)
 * @param   timeoutMs How long to wait at most (-1 for no limit)
 * @param   revents   Where to write what happened (KN_POLL*)
 *
 * @return  1 when something happened, 0 on timeout, -1 on error
 */
///////////////////////////////////////////////////////////////////////////////
int knSocket_poll(knSocket fd, uint32_t events, int timeoutMs, uint32_t *revents);

#endif /* KRONKNET_PLATFORM_SOCKET_H */
