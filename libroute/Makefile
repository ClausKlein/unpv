include ../Make.defines

all:	${LIBUNP_NAME}
${LIBUNP_NAME}:	${LIBROUTE_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

