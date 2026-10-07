/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Poller on Linux (epoll).
*/
#include "../poller.h"
#include "kronknet/macros/errdef.h"
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

    #define KN_POLLER_BATCH 1024  //!< Max events taken from one epoll_wait

struct kronknet_poller_s {

    int                epfd;                     //!< The epoll instance
    struct epoll_event events[KN_POLLER_BATCH];  //!< What epoll_wait reports

};

static uint32_t __knPoller_toEpoll(
    uint32_t events
)
{
    uint32_t ev = 0;

    if (events & KN_POLLIN) ev |= EPOLLIN;
    if (events & KN_POLLOUT) ev |= EPOLLOUT;
    if (events & KN_POLLERR) ev |= EPOLLERR;
    if (events & KN_POLLHUP) ev |= EPOLLHUP;
    return ev;
}

static uint32_t __knPoller_fromEpoll(
    uint32_t ev
)
{
    uint32_t events = 0;

    if (ev & EPOLLIN) events |= KN_POLLIN;
    if (ev & EPOLLOUT) events |= KN_POLLOUT;
    if (ev & EPOLLERR) events |= KN_POLLERR;
    if (ev & EPOLLHUP) events |= KN_POLLHUP;
    return events;
}

knPoller *knPoller_create(void)
{
    knPoller *poller = malloc(sizeof(knPoller));

    if (!poller) {
        return NULL;
    }
    poller->epfd = epoll_create1(EPOLL_CLOEXEC);
    if (poller->epfd == -1) {
        free(poller);
        return NULL;
    }
    return poller;
}

void knPoller_destroy(
    knPoller *poller
)
{
    if (!poller) {
        return;
    }
    close(poller->epfd);
    free(poller);
}

static int __knPoller_ctl(
    knPoller *poller,
    int op,
    knSocket fd,
    void *ptr,
    uint32_t events
)
{
    struct epoll_event ev = {
        .events = __knPoller_toEpoll(events),
        .data.ptr = ptr,
    };

    if (epoll_ctl(poller->epfd, op, fd, &ev) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

int knPoller_add(
    knPoller *poller,
    knSocket fd,
    void *ptr,
    uint32_t events
)
{
    return __knPoller_ctl(poller, EPOLL_CTL_ADD, fd, ptr, events);
}

int knPoller_modify(
    knPoller *poller,
    knSocket fd,
    void *ptr,
    uint32_t events
)
{
    return __knPoller_ctl(poller, EPOLL_CTL_MOD, fd, ptr, events);
}

int knPoller_remove(
    knPoller *poller,
    knSocket fd
)
{
    return __knPoller_ctl(poller, EPOLL_CTL_DEL, fd, NULL, 0);
}

int knPoller_wait(
    knPoller *poller,
    knPollEvent *events,
    int max,
    int timeoutMs
)
{
    int nfds;

    if (max > KN_POLLER_BATCH) {
        max = KN_POLLER_BATCH;
    }
    nfds = epoll_wait(poller->epfd, poller->events, max, timeoutMs);
    for (int i = 0; i < nfds; ++i) {
        events[i].events = __knPoller_fromEpoll(poller->events[i].events);
        events[i].ptr = poller->events[i].data.ptr;
    }
    return nfds;
}
