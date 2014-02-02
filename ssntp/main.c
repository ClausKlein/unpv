#include    "sntp.h"

/** XXX
claus-kleins-macbook-pro:etc clausklein$ tcpdump -i en1 -vv -X port ntp and udp and ip6 multicast
tcpdump: listening on en1, link-type EN10MB (Ethernet), capture size 96 bytes
19:06:56.729622 IP6 (hlim 32, next-header: UDP (17), length: 56) claus-kleins-macbook-pro.local.ntp > ff02::101.ntp: NTPv4, length 48
    Broadcast, Leap indicator: clock unsynchronized (192), Stratum 0, poll 6s, precision -20
    Root Delay: 0.000000, Root dispersion: 0.016708, Reference-ID: (unspec)
      Reference Timestamp:  3569505079.497923284 (2013/02/10 18:11:19)
      Originator Timestamp: 0.000000000 [|ntp]
    0x0000:  6000 0000 0038 1120 fe80 0000 0000 0000  `....8..........
    0x0010:  0223 6cff fe8c 45b1 ff02 0000 0000 0000  .#l...E.........
    0x0020:  0000 0000 0000 00fb 007b 007b 0038 d857  .........{.{.8.W
    0x0030:  e500 06ec 0000 0000 0000 0447 1148 ff0b  ...........G.H..
    0x0040:  d4c2 5337 7f77 e66e 0000 0000 0000 0000  ..S7.w.n........
    0x0050:  0000                                     ..
^C
1 packets captured
32 packets received by filter
0 packets dropped by kernel
claus-kleins-macbook-pro:etc clausklein$
FIXME link-local scoped multicast addr: sudo ./ssntp FF02::101
sudo ./ssntp FF01::101   # interface-local works

OK
sudo ./ssntp 224.0.1.1

XXX **/

int
main(int argc, char **argv) {
    int                 sockfd;
    char                buf[MAXLINE];
    ssize_t             n;
    socklen_t           salen, len;
    struct ifi_info     *ifi;
    struct sockaddr     *mcastsa, *wild, *from;
    struct ifi_info     *ifihead = NULL;
    struct timeval      now;
    const int           on = 1;
    char                *service = {"ntp"};

    if (argc < 2) {
        err_quit("usage: ssntp <IPaddress> [ntp]\nexample: ./ssntp FF01::101 ");
    }
    if (argc > 2) {
        service = argv[2];
    }

    sockfd = Udp_client(argv[1], service, (struct sockaddr **) &mcastsa, &salen);

    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    wild = Malloc(salen);
    memcpy(wild, mcastsa, salen);   /* copy family and port */
//FIXME prevent bind error: Address already in use
    //XXX sock_set_wild(wild, salen);
    Bind(sockfd, wild, salen);  /* bind wildcard */

#ifdef  MCAST
    /* obtain interface list and process each one */
//FIXME: ignore aliases to prevent mcast_join error: Address already in use!
    for (ifihead = ifi = Get_ifi_info(mcastsa->sa_family, 0); ifi != NULL;
            ifi = ifi->ifi_next) {
        if (ifi->ifi_flags & IFF_MULTICAST) {
            Mcast_join(sockfd, mcastsa, salen, ifi->ifi_name, 0);
            printf("joined %s on %s\n",
                   Sock_ntop(mcastsa, salen), ifi->ifi_name);
        }
    }
    if (ifihead) {
        free_ifi_info(ifihead);
    }
#endif

    from = Malloc(salen);
    for (; ;) {
        len = salen;
        n = Recvfrom(sockfd, buf, sizeof(buf), 0, from, &len);
        Gettimeofday(&now, NULL);
        sntp_proc(buf, n, &now);
    }

    return 0;
}
