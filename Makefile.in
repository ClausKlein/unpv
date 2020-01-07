include ./Make.defines

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


all: $(SUBDIRS)

clean:: $(SUBDIRS)
	rm -f $(CLEANFILES)

distclean::
	rm -f $(CLEANFILES) config.cache config.log config.status config.h Make.defines Makefile
	find . -type d -name '*.dSYM' | xargs rm -rf
	find . \( -name 'tags' -o -name '*.d' -o -name '*.o' -o -name '*~' \) -delete

.PHONY: $(SUBDIRS) all clean distclean
$(SUBDIRS):
	${MAKE} -C $@ -w$(MAKEFLAGS) $(MAKECMDGOALS)

# No need to change
$(MAKECMDGOALS):: $(SUBDIRS)

