#include    "sntp.h"

/** from RFC5905#section-14
 * An SNTP client implementing the on-wire protocol has a single server
 * and no dependent clients.  It can operate with any subset of the NTP
 * on-wire protocol, the simplest approach using only the transmit (xmt)
 * timestamp of the server packet and ignoring all other fields.
 *
 * UDP/TCP Port 123 was previously assigned by IANA for this protocol.
 * The IANA has assigned the IPv4 multicast group address 224.0.1.1 and
 * the IPv6 multicast address ending :101 for NTP.
 *
 * For IPv4, this equates to either IPv4 broadcast or IPv4 multicast.  For
 * IPv6, this equates to IPv6 multicast.  For this purpose, IANA has allocated
 * the IPv4 multicast address 224.0.1.1 and the IPv6 multicast address ending
 * :101, with the prefix determined by scoping rules.  Any other non- allocated
 * multicast address may also be used in addition to these allocated multicast
 * addresses.
 *
 * NEWBC.  This indicates a broadcast (mode 5) packet matching no
 * association.  The client mobilizes either a client (mode 3) or
 * broadcast client (mode 6) association.
 *
 * If the implementation supports no additional security or calibration
 * functions, the association mode is set to broadcast client (mode 6)
 *
 **/
void
sntp_proc(char *buf, ssize_t n, struct timeval *nowptr) {
    int             version, mode;
    uint32_t        sec, useci;
    double          usecf;
    struct timeval  diff;
    struct ntpdata  *ntp;

    if (n < (ssize_t)sizeof(struct ntpdata)) {
        printf("\npacket too small: %d bytes\n", n);
        return;
    }

    ntp = (struct ntpdata *) buf;
    version = (ntp->status & VERSION_MASK) >> 3;
    mode = ntp->status & MODE_MASK;
    printf("\nv%d, mode %d (%s), strat %d, ", version, mode,
           (mode == MODE_BROADCAST) ? "broadcast" : "unkown", ntp->stratum);
    if (mode == MODE_CLIENT) {
        printf("client\n");
        return;
    }

    sec = ntohl(ntp->xmt.int_part) - JAN_1970;
    useci = ntohl(ntp->xmt.fraction);   /* 32-bit integer fraction */
    usecf = useci;              /* integer fraction -> double */
    usecf /= 4294967296.0;      /* divide by 2**32 -> [0, 1.0) */
    useci = usecf * 1000000.0;  /* fraction -> parts per million */

    diff.tv_sec = nowptr->tv_sec - sec;
    if ((diff.tv_usec = nowptr->tv_usec - useci) < 0) {
        diff.tv_usec += 1000000;
        diff.tv_sec--;
    }
    useci = (diff.tv_sec * 1000000) + diff.tv_usec; /* diff in microsec */
    printf("clock difference = %d usec\n", useci);
}
