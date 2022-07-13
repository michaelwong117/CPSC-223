#include <stdio.h>
#include <stdlib.h>


int main()
{
	size_t n = 4;
	size_t m = 5;

	int **arr; // this array is a pointer to a pointer to an int because
			   // it's a 2 dimensional array, an array of arrays
			   // incrementing the first pointer (e.g., the first index [])
			   // would point to the next "row", or next array in the array of arrays

	arr = malloc (n * sizeof(*arr)); // malloc, using heap memory
									// can't use dynamic and static arrays together

	for (size_t r = 0; r < n; r++)
	{
		for (size_t c = 0; c < m; c ++)
		{
			arr[r][c] = r * c;
		}
	}

	for (size_t r = 0; r < n; r++)
	{
		for (size_t c = 0; c < m; c ++)
		{
			printf("%d ", arr[r][c]);
		}
		printf("\n");
	}
	for (size_t r = 0; r < n; r++)
	{
		free(arr[r]);
	}
	free(arr);
	// every time there's a malloc there needs to be a free, to deallocate the rows and the pointers to the rows




}