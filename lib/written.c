/* include written */
#include "unp.h"

ssize_t /* Write "n" bytes to a descriptor. */
written(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) {
                nwritten = 0; /* and call write() again */
            } else {
                return (-1); /* error */
            }
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}
/* end written */

void Written(int fd, void *ptr, size_t nbytes) {
    ssize_t toWrite = nbytes;
    if (written(fd, ptr, nbytes) != toWrite) {
        err_sys("written error");
    }
}
