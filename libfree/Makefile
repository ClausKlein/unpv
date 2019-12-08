include ../Make.defines

PROGS:=test_inet_pton test_ascii2addr ### test_getservbyname_r

all:	${LIBUNP_NAME} ${PROGS}
${LIBUNP_NAME}:	${LIBFREE_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

test_inet_pton:	test_inet_pton.o
		${CC} ${CFLAGS} -o $@ test_inet_pton.o ${LIBS}

test_ascii2addr:	test_ascii2addr.o
		${CC} ${CFLAGS} -o $@ test_ascii2addr.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
