#include <time.h>

#include "unp.h"

int main(int argc, char **argv) {
    int sockfd = -1;
    char buff[MAXLINE];
    time_t ticks;
    socklen_t len;
    struct sockaddr_storage cliaddr;

    if (argc == 2) {
        sockfd = Udp_server(NULL, argv[1], NULL);
    } else if (argc == 3) {
        sockfd = Udp_server(argv[1], argv[2], NULL);
    } else {
        err_quit("usage: daytimeudpsrv [ <host> ] <service or port>");
    }

    for (;;) {
        len = sizeof(cliaddr);
        Recvfrom(sockfd, buff, MAXLINE, 0, (SA *)&cliaddr, &len);
        printf("datagram from %s\n", Sock_ntop((SA *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Sendto(sockfd, buff, strlen(buff), 0, (SA *)&cliaddr, len);
    }
}
