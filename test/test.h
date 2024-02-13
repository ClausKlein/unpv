#include "unp.h"

/* globals */
extern struct sockaddr_in servaddr, cliaddr;
extern char buff[BUFFSIZE];
extern int verbose;

int TcpSockByAddr(char *, int);
int UdpSockByAddr(char *, int);
int UdpConnSockByAddr(char *, int);
