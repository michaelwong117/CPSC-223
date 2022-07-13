stack(zoo) is rougly equivalent to 8 megabytes

array of doubles (8 bytes each), 8 million bytes is going to be 8 million doubles

// can't declare more than 8 megabytes of local variables, or if you have multiple functions
// you can run out of stack space 

// on the heap you have as much memory as the system does

// the stack is static allocation, the heap is dyanmic allocation

int **arr; // this array is a pointer to a pointer to an int because
	   // it's a 2 dimensional array, an array of arrays
	   // incrementing the first pointer (e.g., the first index [])
	   // would point to the next "row", or next array in the array of arrays
arr = malloc (n * sizeof(*arr));
