TARGET=kmtest
INCLUDE = -I../
SRCS = kmtest.c
OBJS = ${SRCS:%.c=%.o}

.c.o:
	gcc -c ${INCLUDE} ${CFLAG} $<

all:	${OBJS}
	gcc ${OBJS} -o ${TARGET} ${CFLAG}

run:
	sudo ./${TARGET}

clean:
	rm -f ${OBJS} ${TARGET}
