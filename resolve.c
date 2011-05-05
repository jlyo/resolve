/*
 * resolv - a simple program to resolve a given hostname to a numeric IP
 *          address.
 *
 *
 * Copyright (c) 2011, Jesse Young
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name Jesse Young may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL JESSE YOUNG BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <assert.h>
#include <assert.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 40

static void usage(const char *const argv0) {
    fprintf(stderr, "Usage: %s [-4|-6] HOSTNAME\n", argv0);
    fprintf(stderr, "Resolve a hostname to numeric IP addresses\n\n");
    fprintf(stderr, "  -4 Use IPv4\n");
    fprintf(stderr, "  -6 Use IPv6\n");
}


int main(const int argc, char *const argv[]) {
    int err;
    int optchar;
    bool ipv6 = false;
    bool ipv4 = false;
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    struct addrinfo *elem;
    const char *hostname;
    char buffer[BUFFER_SIZE];

    while ((optchar = getopt(argc, argv, "64")) != -1) {
        switch(optchar) {
            case '4':
                ipv4 = true;
                break;
            case '6':
                ipv6 = true;
                break;
            case '?':
                usage(argv[0]);
                exit(EXIT_FAILURE);
                break;
            default:
                assert(false);
                break;
        }
    }

    if (optind >= argc) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    hostname = argv[optind];

    hints.ai_family = ipv4 == ipv6
                    ? AF_UNSPEC
                    : ipv4 ? AF_INET : AF_INET6;
    /* socktype and proto are arbitrary */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    /* "1" is arbitrary */
    if ((err = getaddrinfo(hostname, "1", &hints, &res)) != 0) {
        fprintf(stderr, "%s: %s\n", argv[0], gai_strerror(err));
        if (res != NULL) {
            freeaddrinfo(res);
        }
        exit(EXIT_FAILURE);
    }

    for (elem = res; elem != NULL; elem = elem->ai_next) {
        if ((err = getnameinfo(elem->ai_addr, elem->ai_addrlen,
                        buffer, BUFFER_SIZE, NULL, 0,
                        NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
            fprintf(stderr, "%s: %s\n", argv[0], gai_strerror(err));
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buffer);
    }

    freeaddrinfo(res);
    return EXIT_SUCCESS;
}
