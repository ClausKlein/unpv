#include	"sntp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	char				buf[MAXLINE];
	ssize_t				n;
	socklen_t			salen, len;
	struct ifi_info		*ifi;
	struct sockaddr		*mcastsa, *wild, *from;
	struct timeval		now;
	const int			on = 1;
	char				*service = {"ntp"};

	if (argc < 2)
		err_quit("usage: ssntp <IPaddress> [ntp]");
	if (argc > 2)
	service = argv[2];

	sockfd = Udp_client(argv[1], service, (struct sockaddr **) &mcastsa, &salen);

	Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	wild = Malloc(salen);
	memcpy(wild, mcastsa, salen);	/* copy family and port */
	sock_set_wild(wild, salen);

	Bind(sockfd, wild, salen);	/* bind wildcard */

#ifdef	MCAST
	/* obtain interface list and process each one */
	for (ifi = Get_ifi_info(mcastsa->sa_family, 1); ifi != NULL;
		ifi = ifi->ifi_next) {
		if (ifi->ifi_index == 1) continue; // FIXME do not use the loopback device
		printf("interface#%d %s\n", ifi->ifi_index, ifi->ifi_name );
		if (ifi->ifi_flags & IFF_MULTICAST) {
			Mcast_join(sockfd, mcastsa, salen, ifi->ifi_name, 0);
			printf("joined %s on %s\n",
				   Sock_ntop(mcastsa, salen), ifi->ifi_name);
		}
	}
#endif

	from = Malloc(salen);
	for ( ; ; ) {
		len = salen;
		n = Recvfrom(sockfd, buf, sizeof(buf), 0, from, &len);
		Gettimeofday(&now, NULL);
		sntp_proc(buf, n, &now);
	}

	return 0;
}
