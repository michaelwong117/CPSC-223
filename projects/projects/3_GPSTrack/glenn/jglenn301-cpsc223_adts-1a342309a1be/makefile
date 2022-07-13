CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g3

TSPEast: tsp_east.o more_math.o location.o location_list.o
	gcc -o $@ -g $^ -lm

tsp_east.o: location.h location_list.h
more_math.o:  more_math.h
location.o: location.h more_math.h
location_list.o: location.h location_list.h
