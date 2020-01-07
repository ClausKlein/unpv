#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#if defined BSD || defined __APPLE__
#include <net/if_dl.h>
#include <net/if_types.h> // IFT_ETHER
#endif

#include <ifaddrs.h> // getifaddrs
#include <netdb.h>   // getnameinfo

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
#if defined BSD || defined __APPLE__
    struct ifaddrs *ifaddr, *ifa;
    int family, len, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        family = ifa->ifa_addr->sa_family;
        len    = ifa->ifa_addr->sa_len;

        /* Display interface name and family (including symbolic
           form of the latter for the common families) */

        printf("%s\taddress family: %d%s\n", ifa->ifa_name, family,
            // FIXME (family == AF_PACKET) ? " (AF_PACKET)" :
            (family == AF_LINK) ? " (AF_LINK)"
                                : (family == AF_INET)
                    ? " (AF_INET)"
                    : (family == AF_INET6) ? " (AF_INET6)" : "");

        /* For an AF_INET* interface address, display the address */

        if (family == AF_INET || family == AF_INET6) {

            // XXX assert(len == ((family == AF_INET) ? sizeof(struct
            //          sockaddr_in) : sizeof(struct sockaddr_in6)));

            s = getnameinfo(
                ifa->ifa_addr, len, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("\t%s\n", host);
        }

        if (family == AF_LINK) {
            struct sockaddr_dl* sdlptr = (struct sockaddr_dl*)ifa->ifa_addr;
            if (sdlptr->sdl_type == IFT_ETHER && sdlptr->sdl_alen) {
                printf("\tether %s\n", link_ntoa((sdlptr)));
            }
        }
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
#endif
}

/***
Claus-MacBook-Pro:ioctl clausklein$ ./getifaddrs
lo0	address family: 18 (AF_LINK)
lo0	address family: 30 (AF_INET6)
        ::1
lo0	address family: 2 (AF_INET)
        127.0.0.1
lo0	address family: 30 (AF_INET6)
        fe80::1%lo0
lo0	address family: 30 (AF_INET6)
        ::ffff:1.2.3.4
gif0	address family: 18 (AF_LINK)
stf0	address family: 18 (AF_LINK)
en0	address family: 18 (AF_LINK)
        ether en0:0.23.32.b8.bd.9a
en1	address family: 18 (AF_LINK)
        ether en1:0.23.6c.8c.45.b1
en1	address family: 30 (AF_INET6)
        fe80::223:6cff:fe8c:45b1%en1
en1	address family: 2 (AF_INET)
        192.168.1.154
en1	address family: 30 (AF_INET6)
        fd00::223:6cff:fe8c:45b1
en1	address family: 30 (AF_INET6)
        fd00::4c66:f1f5:4dd9:3cf7
fw0	address family: 18 (AF_LINK)
p2p0	address family: 18 (AF_LINK)
        ether p2p0:2.23.6c.8c.45.b1
Claus-MacBook-Pro:ioctl clausklein$
 ***/
