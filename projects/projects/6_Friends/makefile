CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

Friends: friends.o graph.o string_key.o gmap.o gqueue.o
	${CC} ${CFLAGS} -o $@ $^ -lm

# put everything in GraphUnit
GraphUnit: graph_unit.o graph.o string_key.o gmap.o gqueue.o
	${CC} -o $@ ${CFLAGS} $^ -lm

graph.o: graph.h gmap.h string_key.h gqueue.h
gmap.o: gmap.h
string_key.o: string_key.h
gqueue.o: gqueue.h
