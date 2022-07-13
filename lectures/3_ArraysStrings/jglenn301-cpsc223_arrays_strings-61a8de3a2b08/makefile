CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

Arrays: arrays.o
	${CC} ${CFLAGS} -o $@ $^

Strings: strings.o
	${CC} ${CFLAGS} -o $@ $^

Strlen: time_strlen.o
	${CC} ${CFLAGS} -o $@ $^
