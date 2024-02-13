#include <sys/param.h>
#include <sys/ucred.h>

#include "unp.h"

int main() {
    printf("sizeof(struct fcred) = %d\n", sizeof(struct fcred));
    printf("sizeof(struct cmsghdr) = %d\n", sizeof(struct cmsghdr));
    return 0;
}
