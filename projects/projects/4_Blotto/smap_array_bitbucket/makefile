CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

Monitor: monitor.o smap_chained.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

WordCount: word_count.o smap_chained.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

CountFlights: count_flights.o smap_chained.o string_util.o usdot.o
	${CC} ${CFLAGS} -o $@ $^

word_count.o: smap.h
monitor.o: smap.h
smap_chained.o: smap.h
count_flights.o: smap.h usdot.h string_util.h
