export CC=gcc

-include ./Make.defines

UNAME=$(shell uname)

SUBDIRS:= ./lib ./libfree ./advio ./bcast ./inetd ./intro ./ioctl ./ipopts ./mcast \
./mysdr ./names ./nonblock ./oob ./ping ./rtt ./select ./server ./sigio ./sock ./sockopt \
./ssntp ./tcpcliserv ./udpcliserv ./unixdomain ./icmpd ./test ./threads

ifeq ($(UNAME),Darwin)
###FIXME build errors on Darwin
### SUBDIRS+= ./debug ./sctp ./streams ./libgai ./udpcksum
	SUBDIRS+= ./key ./libroute ./traceroute ./route
else
###FIXME build errors on Linux
### SUBDIRS+= ./debug ./key ./route ./sctp ./streams \
### ./libgai ./libroute ./traceroute ./udpcksum
endif


.PHONY: $(SUBDIRS) setup init all clean distclean
all: config.cache $(SUBDIRS)

setup: config.cache
config.cache:
	./configure --config-cache
	${MAKE} -C lib -w$(MAKEFLAGS)

Make.defines: Make.defines.in
config.h: config.h.in

clean: $(SUBDIRS)

distclean:
	${MAKE} -C . -w$(MAKEFLAGS) clean
	rm -f $(CLEANFILES) config.cache config.log config.status config.h Makefile Make.defines
	find . -type d -name '*.dSYM' | xargs rm -rf
	find . \( -name 'tags' -o -name '*.d' -o -name '*.o' -o -name '*~' \) -delete

$(SUBDIRS): init
	${MAKE} -C $@ -w$(MAKEFLAGS) $(MAKECMDGOALS)

init:
	${MAKE} -C lib -w$(MAKEFLAGS)

# No need to change
$(MAKECMDGOALS): $(SUBDIRS)

.NOTPARALLEL: setup init
