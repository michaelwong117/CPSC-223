all: Stderr Residuals Strings

Stderr: stderr.o
	gcc -o Stderr -Wall -std=c99 -pedantic -g stderr.o

stderr.o: stderr.c
	gcc -c -Wall -std=c99 -pedantic -g stderr.c

Residuals: residuals.o
	gcc -o Residuals -Wall -std=c99 -pedantic -g residuals.o

residuals.o: residuals.c
	gcc -c -Wall -std=c99 -pedantic -g residuals.c

Strings: strings.o
	gcc -o Strings -Wall -std=c99 -pedantic -g strings.o

strings.o: strings.c
	gcc -c -Wall -std=c99 -pedantic -g strings.c
