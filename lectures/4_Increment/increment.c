#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int increment(int x);
int increment_actual(int *x); // these all have to be pointers

int main(int argc, char *argv[])
{
	int a = 10;
	increment(a); // returns a=10 because there's no reference to memory address of a
	printf("a=%d\n", a);

	increment_actual(&a); // must apply to the reference of the variable
	printf("a=%d\n", a);

	// if you increment &a, the reference of a

	increment(&a); 

	// you are incrementing the memory address of a, which is not what you want
	// e.g., memory address 408 of a to 412 of something undefined
	
}

int increment(int x) // returns a=10
{
	x = x+1;
}

int increment_actual(int *x)
{
	*x = *x+1; // pointer to memory address
}