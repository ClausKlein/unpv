export CC=gcc

-include ./Make.defines

UNAME=$(shell uname)

SUBDIRS:= ./lib ./libfree ./advio ./bcast ./inetd ./intro ./ioctl ./ipopts \
./names ./oob ./rtt ./select ./sigio ./sock ./sockopt \
./ssntp ./tcpcliserv ./udpcliserv ./unixdomain ./test

ifeq ($(UNAME),Darwin)
###FIXME build errors on Darwin
### SUBDIRS+= ./debug ./sctp ./streams ./libgai ./udpcksum ./traceroute
	SUBDIRS+= ./key ./libroute ./route
else
###FIXME build errors on Linux
SUBDIRS+= ./mcast ./mysdr ./threads
### SUBDIRS+= ./nonblock ./ping ./server ./icmpd ./debug ./key ./route ./sctp ./streams \
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
