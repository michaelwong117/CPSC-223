CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g3

TSPEast: tsp_east.o more_math.o location.o
	gcc -o $@ -g $^ -lm

tsp_east.o: location.h
more_math.o:  more_math.h
location.o: location.h more_math.h
