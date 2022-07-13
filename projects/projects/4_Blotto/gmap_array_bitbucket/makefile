CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

CountFlights: count_flights.o gmap_array.o string_key.o string_util.o usdot.o
	${CC} ${CFLAGS} -o $@ $^

count_flights.o: gmap.h string_key.h string_util.h usdot.h
gmap_array.o: gmap.h string_key.h
