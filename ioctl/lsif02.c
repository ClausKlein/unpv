#include <net/if.h>

#include "unp.h"

#ifdef HAVE_SOCKADDR_DL_STRUCT
#    include <net/if_dl.h>
#    include <net/if_types.h>
#endif

#if 0

// from /usr/include/sys/types.h:
// typedef char * caddr_t;        /* core address */

// from /usr/include/sys/socket.h:
/*
 * [XSI] Structure used by kernel to store most addresses.
 */
struct sockaddr {
    __uint8_t   sa_len;         /* total length */
    sa_family_t sa_family;      /* [XSI] address family */
    char        sa_data[14];    /* [XSI] addr value (actually larger) */
};

// from /usr/include/net/if.h:

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct  ifreq {
#    ifndef IFNAMSIZ
#        define IFNAMSIZ IF_NAMESIZE
#    endif
    char    ifr_name[IFNAMSIZ];             /* if name, e.g. "en0" */
    union {
        struct  sockaddr ifru_addr;
        struct  sockaddr ifru_dstaddr;
        struct  sockaddr ifru_broadaddr;
        short   ifru_flags;
        int     ifru_metric;
        int     ifru_mtu;
        int     ifru_phys;
        int     ifru_media;
        int     ifru_intval;
        caddr_t ifru_data;
        struct  ifdevmtu ifru_devmtu;
        struct  ifkpi   ifru_kpi;
        u_int32_t ifru_wake_flags;
        u_int32_t ifru_route_refcnt;
        int     ifru_cap[2];
    } ifr_ifru;
#    define ifr_addr ifr_ifru.ifru_addr           /* address */
#    define ifr_dstaddr ifr_ifru.ifru_dstaddr     /* other end of p-to-p link */
#    define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast address */
#    ifdef __APPLE__
#        define ifr_flags ifr_ifru.ifru_flags /* flags */
#    else
#        define ifr_flags ifr_ifru.ifru_flags[0]     /* flags */
#        define ifr_prevflags ifr_ifru.ifru_flags[1] /* flags */
#    endif                                           /* __APPLE__ */
#    define ifr_metric ifr_ifru.ifru_metric          /* metric */
#    define ifr_mtu ifr_ifru.ifru_mtu                /* mtu */
#    define ifr_phys ifr_ifru.ifru_phys              /* physical wire */
#    define ifr_media ifr_ifru.ifru_media            /* physical media */
#    define ifr_data ifr_ifru.ifru_data              /* for use by interface */
#    define ifr_devmtu ifr_ifru.ifru_devmtu
#    define ifr_intval ifr_ifru.ifru_intval /* integer value */
#    define ifr_kpi ifr_ifru.ifru_kpi
#    define ifr_wake_flags ifr_ifru.ifru_wake_flags /* wake capabilities */
#    define ifr_route_refcnt \
        ifr_ifru.ifru_route_refcnt          /* route references count */
#    define ifr_reqcap ifr_ifru.ifru_cap[0] /* requested capabilities */
#    define ifr_curcap ifr_ifru.ifru_cap[1] /* current capabilities */
};

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */
struct  ifconf {
    int     ifc_len;                    /* size of associated buffer */
    union {
        caddr_t ifcu_buf;
        struct  ifreq *ifcu_req;
    } ifc_ifcu;
};
#    define ifc_buf ifc_ifcu.ifcu_buf       /* buffer address */
#    define ifc_req ifc_ifcu.ifcu_req       /* array of structures returned */
#endif

int main(int argc, char **argv) {
    int sockfd, len;
    char *ptr, buf[2048], addrstr[INET_ADDRSTRLEN];
    struct ifconf ifc;
    struct ifreq *ifr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        err_sys("socket error");
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_req = (struct ifreq *)buf;
    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
        err_sys("ioctl SIOCGIFCONF error");
    }

    for (ptr = buf; ptr < buf + ifc.ifc_len;) {
        ifr = (struct ifreq *)ptr;
        len = sizeof(struct sockaddr);
#ifdef HAVE_SOCKADDR_SA_LEN
        if (ifr->ifr_addr.sa_len > len) {
            len = ifr->ifr_addr.sa_len; /* length > 16 */
        }
#endif
        ptr += sizeof(ifr->ifr_name) + len; /* for next one in buffer */

        switch (ifr->ifr_addr.sa_family) {
            case AF_INET: {
                struct sockaddr_in *sinptr;
                sinptr = (struct sockaddr_in *)&ifr->ifr_addr;
                printf("%s\t%s\n", ifr->ifr_name,
                       Inet_ntop(AF_INET, &sinptr->sin_addr, addrstr,
                                 sizeof(addrstr)));
                break;
            }

#ifdef AF_INET6
            case AF_INET6: {
                struct sockaddr_in6 *sin6ptr;
                char addr6str[INET6_ADDRSTRLEN];

                sin6ptr = (struct sockaddr_in6 *)&ifr->ifr_addr;
                printf("%s\t%s\n", ifr->ifr_name,
                       Inet_ntop(AF_INET6, &sin6ptr->sin6_addr, addr6str,
                                 sizeof(addr6str)));
                break;
            }
#endif

#ifdef HAVE_SOCKADDR_DL_STRUCT
            case AF_LINK: {
                struct sockaddr_dl *sdlptr;
                char str[18];
                char *etherprint(const u_char *, char *);

                sdlptr = (struct sockaddr_dl *)&ifr->ifr_addr;
                printf("%s", ifr->ifr_name);
                if (sdlptr->sdl_index) {
                    printf("\t<link %d>", sdlptr->sdl_index);
                }
                if (sdlptr->sdl_type == IFT_ETHER && sdlptr->sdl_alen) {
                    printf("\t%s", etherprint((u_char *)LLADDR(sdlptr), str));
                }
                putchar('\n');
                break;
            }
#endif

            default:
                printf("%s\n", ifr->ifr_name);
                break;
        }
    }
    exit(0);
}

#ifdef HAVE_SOCKADDR_DL_STRUCT
char *etherprint(const u_char eaddr[6], char string[18]) {
    snprintf(string, 18, "%02x:%02x:%02x:%02x:%02x:%02x", eaddr[0], eaddr[1],
             eaddr[2], eaddr[3], eaddr[4], eaddr[5]);
    string[17] = '\0';
    return (string);
}
#endif
