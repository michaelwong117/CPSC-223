Distance: distance.o more_math.o
	gcc -o Distance -g distance.o more_math.o -lm

distance.o: distance.c more_math.h
	gcc -c -Wall -std=c99 -pedantic -g distance.c

more_math.o: more_math.c more_math.h
	gcc -c -Wall -std=c99 -pedantic -g more_math.c
