include ./Make.defines

SUBDIRS:= ./lib ./libfree ./advio ./bcast ./inetd ./intro ./ioctl ./ipopts ./mcast \
./mysdr ./names ./nonblock ./oob ./ping ./rtt ./select ./server ./sigio ./sock ./sockopt \
./ssntp ./tcpcliserv ./udpcliserv ./unixdomain

###FIXME build errors on Darwin
## SUBDIRS+= ./debug ./sctp ./streams ./libgai ./udpcksum
SUBDIRS+= ./icmpd ./key ./test ./libroute ./traceroute ./route ./threads

###FIXME build errors on Linux
## SUBDIRS+= ./debug ./icmpd ./key ./route ./sctp ./streams \
## ./test ./threads ./libgai ./libroute ./traceroute ./udpcksum

###FIXME build errors on Cygwin: libfree
# SUBDIRS+= lib intro mysdr mcast ssntp

all: $(SUBDIRS)

clean:: $(SUBDIRS)
	rm -f $(CLEANFILES)

distclean::
	rm -f $(CLEANFILES) config.cache config.log config.status ### config.h Make.defines Makefile
	find . -type d -name '*.dSYM' | xargs rm -rf
	find . \( -name 'tags' -o -name '*.d' -o -name '*.o' -o -name '*~' \) -delete

.PHONY: $(SUBDIRS) all clean distclean
$(SUBDIRS):
	${MAKE} -C $@ -w$(MAKEFLAGS) $(MAKECMDGOALS)

# No need to change
$(MAKECMDGOALS):: $(SUBDIRS)

