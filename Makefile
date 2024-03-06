CC = cc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -pedantic-errors -ggdb -I. -Wshadow

SRCS = builtins.c eval.c parser.c lexer.c util.c forth.c
OBJS = ${SRCS:.c=.o}
TARGET = forth

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${TARGET}

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f ${OBJS} ${TARGET}

