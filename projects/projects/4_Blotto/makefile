CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

Blotto: blotto.o gmap.o entry.o string_key.o gmap_test_functions.o 
	${CC} ${CFLAGS} -o $@ $^ -lm

GmapUnit: gmap_unit.o gmap.o string_key.o gmap_test_functions.o
# 	gcc -o $@ -g $^ -lm
	${CC} -o $@ ${CFLAGS} $^ -lm


gmap_unit.o: gmap_unit.c gmap_test_functions.h string_key.h gmap.h
gmap_test_functions.o: gmap_test_functions.h
string_key.o: string_key.h
entry.o: entry.h
gmap.o: gmap.h


