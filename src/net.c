#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

#include <event2/event.h>

#include "log.h"
#include "net.h"

evutil_socket_t
new_receiver(const char *addr, const char *port)
{
        evutil_socket_t sock;
        struct addrinfo *res, hints;
        int rv, s;

        int optval;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_addrlen = 0;
        hints.ai_addr = NULL;
        hints.ai_canonname = NULL;
        hints.ai_next = NULL;
        hints.ai_flags = AI_PASSIVE;

        if ((rv = getaddrinfo(NULL, port, &hints, &res)) != 0) {
                log_error("getaddrinfo failed");
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                sock = -1;
                goto cleanup;
        }

        if ((sock =
             socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
                log_error("socket failed");
                sock = -1;
                goto cleanup;
        }

        evutil_make_socket_nonblocking(sock);
        optval = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
            < 0) {
                log_error("setsockopt failed");
                sock = -1;
                goto cleanup;
        }

        if (bind(sock, res->ai_addr, res->ai_addrlen) < 0) {
                log_error("bind failed");
                sock = -1;
                goto cleanup;
        }

 cleanup:
        if (res != NULL)
                freeaddrinfo(res);

        return sock;
}
