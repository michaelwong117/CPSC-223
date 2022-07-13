CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

PointsetUnit: pointset_unit.o pointset.o point2d.o pqueue.o #minheap.o
# 	gcc -o $@ -g $^ -lm
	${CC} -o $@ ${CFLAGS} $^ -lm

pointset_unit.o: pointset_unit.c point2d.h pointset.h
pointset.o: pointset.h point2d.h pqueue.h #minheap.h
pqueue.o: pqueue.h
# minheap.o: minheap.h
point2d.o: point2d.h