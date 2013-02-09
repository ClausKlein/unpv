#include	"unp.h"

void	recv_all(int, socklen_t);
void	send_all(int, SA *, socklen_t);

int
main(int argc, char **argv)
{
	int					sendfd, recvfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sasend, *sarecv;

	if (argc != 3)
		err_quit("usage: sendrecv <IP-multicast-address> <port#>");

	sendfd = Udp_client(argv[1], argv[2], (struct sockaddr **) &sasend, &salen);

	recvfd = Socket(sasend->sa_family, SOCK_DGRAM, 0);

	Setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	sarecv = Malloc(salen);
	memcpy(sarecv, sasend, salen);

#if !defined(__APPLE__) && !defined(__BSD__)
#warning "bind to any addr"
	//FIXME bind error: Cannot assign requested address
	sock_set_wild(sarecv, salen);
#endif

	Bind(recvfd, sarecv, salen);

	//XXX if (mcast_leave(recvfd, sasend, salen) < 0) perror ("mcast_leave");
	Mcast_join(recvfd, sasend, salen, NULL, 0);

	//FIXME we can't receive our multicast mesages send under apple, linux and cygwin
	// example: mcast/sendrecv 224.0.0.251 5353
	//     and: mcast/sendrecv FF02::FB 5353
	Mcast_set_loop(sendfd, on);

	if (Fork() == 0)
		recv_all(recvfd, salen);		/* child -> receives */

	send_all(sendfd, sasend, salen);	/* parent -> sends */

	return 0;
}
