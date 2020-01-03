include ../Make.defines

PROGS =	lsif01 lsif02 prifinfo prmac getifaddrs ifconfig

all:	${PROGS}

lsif01:	lsif01.o
		${CC} ${CFLAGS} -o $@ lsif01.o ${LIBS}

lsif02:	lsif02.o
		${CC} ${CFLAGS} -o $@ lsif02.o ${LIBS}

prifinfo:	prifinfo.o
		${CC} ${CFLAGS} -o $@ prifinfo.o ${LIBS}

prmac:	prmac.o
		${CC} ${CFLAGS} -o $@ prmac.o ${LIBS}

getifaddrs:	getifaddrs.o
		${CC} ${CFLAGS} -o $@ getifaddrs.o ${LIBS}

ifconfig:	ifconfig.o
		${CC} ${CFLAGS} -o $@ ifconfig.o ${LIBS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

