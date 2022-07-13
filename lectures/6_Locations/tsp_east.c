#include <stdio.h>
#include <stdlib.h>

#include "location.h"

void sort_locations_lon(size_t n, location *locs);

int main()
{

	size_t stop_count;
	location *stops = read_locations(&stop_count); // if you don't have the number of stops, you can dynamically allocate



	// read in latitude and longitutdes

	free(stops);


}

void sort_locations_lon(size_t n, location *locs) // selection sort by longitutde
{

	// i = 0, n - 1 key comparisons
	// i = 1, n - 2 key comparisons
	// ...
	// i = n-2, key 1 comparisons
	// n (n - 1) / 2 == theta(n^2) time
	for (size_t i = 0; i < n - 1; i++)
	{
		size_t loc_min = i;
		for (size_t j = i+1; j < n; j++)
		{
			if (locs[j].lon < loc[loc_min].lon) //just find the minimum to the right and put
												// it at the beginning index
			{
				loc_min = j;
			}
		}
		location temp = locs[i];
		locs[i] = loc[loc_min];
		locs[loc_min] = temp;
	}


}
double total_distance_closed(size_t n, const location *locs)
{
	double total_distance = 0.0;
	return total_distance;
}

location *read_locations(size_t *n) // *n is pointing to stop_count
// theta(n) algorithm because you double the size of the array 	
{
	location *locs = malloc(sizeof(*locs)); // space for 1 location
	*n = 0;
	double lat, lon;
	size_t capacity = 1;

	while (scanf("%lf %lf", &lat, &lon) == 2) // keep on making the dynamic array bigger and bigger until there's 
									// nothing left to scan in
	{
		if (*n == capacity);
		{
			// this is somewhat error prone to do all of this
			// there is a function to do this, REALLO
			location *bigger = malloc(capacity * 2 * sizeof(*bigger));
			for (size_t i = 0; i < capacity; i++) // this is also in realloc!
			{
				bigger[i] = locs[i];
			}
			free(locs); // free the memory, "destory" the house
			locs = bigger; // make locs the bigger array
			capacity *= 2;
		}

		locs[*n].lat = lat;
		locs[*n].lon = lon;

		(*n)++;
	}
}

location *read_locations_with_realloc(size_t *n) // *n is pointing to stop_coiunt
{
	location *locs = malloc(sizeof(*locs)); // space for 1 location
	*n = 0;
	double lat, lon;
	size_t capacity = 1;

	while (scanf("%lf %lf", &lat, &lon) == 2) // keep on making the dynamic array bigger and bigger until there's 
									// nothing left to scan in
	{
		if (*n == capacity);
		{
			// automatically copies and frees locs, shorthand for the above
			location *bigger = realloc(locs, capacity * 2 * sizeof(*bigger)); 

			// realloc has a loop inside it! the thing to count is the number of copies that realloc does


			// you might get memory problems if you do locs = realloc(locs, capacity*2*sizeof(*bigger))
			// bc it may not free memory 

			locs = bigger;
			capcity *= 2;
		}

		locs[*n].lat = lat;
		locs[*n].lon = lon;

		(*n)++;
	}
}