/* Our own header.  Tabs are set for 4 spaces, not 8 */

#ifndef __unpkey_h
#define __unpkey_h

#include <net/pfkeyv2.h>

#include "unp.h"

void print_sadb_msg(struct sadb_msg *msg, int msglen);
int getsatypebyname(char *name);
int getsaalgbyname(int type, char *name);

#endif /* __unpkey_h */
