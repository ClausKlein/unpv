#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {
#ifdef __BSD__
    struct in6_addr foo;

    printf("ascii2addr returned %d\n",
           ascii2addr(AF_INET6, "::140.252.13.36", &foo));

    exit(0);
#endif
}
