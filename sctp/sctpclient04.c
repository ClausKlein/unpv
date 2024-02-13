#include "unp.h"

int main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in servaddr;
    struct sctp_event_subscribe events;

    if (argc != 2) err_quit("Missing host argument - use '%s host'\n", argv[0]);
    sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    /* include mod_client04 */
    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    events.sctp_association_event = 1;
    Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));

    sctpstr_cli(stdin, sock_fd, (SA *)&servaddr, sizeof(servaddr));
    /* end mod_client04 */
    close(sock_fd);
    return (0);
}
