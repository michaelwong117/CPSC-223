CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g

GPS: GPS.o more_math.o location.o trackpoint.o track.o
	gcc -o $@ -g $^ -lm

TrackUnit: track_unit.o more_math.o location.o trackpoint.o track.o
# 	gcc -o $@ -g $^ -lm
	${CC} -o $@ ${CFLAGS} $^ -lm

# eventually need to handle TrackUnit executable

# GPS.o: location.h track.h trackpoint.h
track_unit.o: track_unit.c
more_math.o:  more_math.h
location.o: location.h more_math.h
track.o: location.h trackpoint.h track.h
trackpoint.o: location.h trackpoint.h
