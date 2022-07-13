CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g3

# don't mess with file names in makefile, you'll accidently give yourself undefined reference errors
# keep to original filenames

TSP: TSP.o location.o # equivalent gcc command: gcc -o TSP_basic.c location.c
	gcc -o $@ -g $^ -lm -O3
# 	gcc -o $@ -g $^ -lm

TSP.o: location.h
location.o: location.h

