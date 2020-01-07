#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
#ifdef __BSD__
    struct in6_addr foo;

    printf("ascii2addr returned %d\n",
           ascii2addr(AF_INET6, "::140.252.13.36", &foo));

    exit(0);
#endif
}
