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


.PHONY: $(SUBDIRS) setup all clean distclean
setup:: Make.defines config.h

all: setup $(SUBDIRS)

Make.defines: config.h
config.h: configure config.h.in
	./configure --config-cache

clean:: $(SUBDIRS)
	rm -f $(CLEANFILES)

distclean::
	rm -f $(CLEANFILES) config.cache config.log config.status config.h Makefile #NO! Make.defines
	find . -type d -name '*.dSYM' | xargs rm -rf
	find . \( -name 'tags' -o -name '*.d' -o -name '*.o' -o -name '*~' \) -delete

$(SUBDIRS)::
	${MAKE} -C $@ -w$(MAKEFLAGS) $(MAKECMDGOALS)

# No need to change
$(MAKECMDGOALS):: $(SUBDIRS)

