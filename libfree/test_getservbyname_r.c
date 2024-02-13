#include <netdb.h>

#ifdef REENTRANT
int getservbyname_r(const char *name, const char *proto,
                    struct servent *result_buf, char *buf, size_t buflen,
                    struct servent **result);
#endif

int main() {
    char buf[8192];
    struct servent sent, *sptr;

    sptr = getservbyname_r("tftp", "tcp", &sent, buf, sizeof(buf));
    printf("TCP, sptr = %p\n", sptr);

    sptr = getservbyname_r("tftp", "udp", &sent, buf, sizeof(buf));
    printf("UDP, sptr = %p\n", sptr);

    sptr = getservbyname_r("tftp", "tcp", &sent, buf, sizeof(buf));
    printf("TCP, sptr = %p\n", sptr);

    sptr = getservbyname_r("tftp", "udp", &sent, buf, sizeof(buf));
    printf("UDP, sptr = %p\n", sptr);

    exit(0);
}
