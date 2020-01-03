include ../Make.defines

all:	${LIBUNP_NAME}
$(LIBUNP_NAME):	${LIB_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

