/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Portability macros for compiler-specific attributes.
*/

#ifndef KRONKNET_MACROS_PLATFORM_H
    #define KRONKNET_MACROS_PLATFORM_H

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #define KN_NOSIGNAL 0
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/poll.h>
    #include <fcntl.h>
    #include <asm-generic/errno-base.h>
    #include <asm-generic/errno.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <unistd.h>

    #define KN_NOSIGNAL MSG_NOSIGNAL
#endif

#endif /* KRONKNET_MACROS_PLATFORM_H */
