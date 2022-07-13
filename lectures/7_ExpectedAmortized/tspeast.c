#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "location.h"

/*
 * Reads locations (latitude and longitude in degrees) from standard
 * input, one per line, and calculates the length (in miles) of the tour
 * constructed by visiting those points in order of increasing
 * longitude (with ties broken by increasing latitude).  For example,
 * if standard input is
 *
 * 41.263889 -72.886944
 * 26.536111 -81.755278
 * 26.683056 -80.095556
 * 27.975556 -82.533333
 * 26.072500 -80.152778
 * 28.429444 -81.308889
 *
 * then the output is
 *
 * distance 2615.125233 miles
 */

/**
 * Sorts the given array of locations in order of increasing longitude,
 * with ties broken by increasing latitude.
 *
 * @param n a nonnegative integer
 * @param locs a pointer to an array of n locations
 */
void sort_locations_lon(size_t n, location *locs);

/**
 * Reads locations from standard input and returns them in an array,
 * with n set to the number of locations stored in that array.
 * Standard input should contain one line per location, with a latitude
 * and longitude on each line, separated by whitespace.  This function
 * will read until no more locations can be read.  If there is a memory
 * allocation failure then the number of locations returned may be less
 * than the number of locations read.  If any latitude or longitude is
 * invalid, then locations are still read until non-numeric data is
 * encountered, but the returned array is NULL and n is zero.
 *
 * @param n a pointer to an unsigned int for the number of returned locations
 * @return a pointer to an array of locations
 */
location *read_locations(size_t *n);

/**
 * Computes the distance of the tour corresponding to the given array.
 * The tour is closed by returning from the last location in the array
 * to the first.
 *
 * @param n a nonnegative integer
 * @param locs a pointer to an array of n valid locations
 * @return the distance of the corresponding tour
 */
double total_distance_closed(size_t n, const location *locs);



/**
 * Computes the distance of a path given the latitudes and longitudes
 * of the points on the path.  The coordinates are read from standard input
 * and assumed to be in degrees.  The output is the number of points in the
 * path and the total distance using the great circle distance between adjacent
 * points.
 *
 * @version 0.1 2017-09-01
 */
int main()
{
  // read in latitude and longitudes
  size_t stop_count;
  location *stops = read_locations(&stop_count);
  if (stops == NULL)
    {
      return 1;
    }

  sort_locations_lon(stop_count, stops);
  
  double total_distance = total_distance_closed(stop_count, stops);

  for (size_t i = 0; i < stop_count; i++)
  {
    location_destroy(stops[i]);
  }
  printf("distance %lf miles\n", total_distance);

  free(stops);
}

void sort_locations_lon(size_t n, location *locs)
{
  // lazy: selection sort of locations by increasing longitude
  for (size_t i = 0; i < n - 1; i++)
    {
      size_t loc_max = i;
      for (size_t j = i + 1; j < n; j++)
    	{
        if (location_compare(locs[j], locs[loc_max]) > 0)
        {
          loc_max = j;
        }
    	
    	}
          location *temp = locs[i]; // in order to create the size for this object on the stack
                              // needs to know the size to create space on the stack
      locs[i] = locs[loc_max];
      locs[loc_max] = temp;
    }
}

double total_distance_closed(size_t n, const location *locs)
{
  double total_distance = 0.0;
  for (size_t i = 0; i < n; i++)
    {
      total_distance += location_distance(locs[i], locs[(i + 1) % n]);
    }
  return total_distance;
}

location *read_locations(size_t *n)
{
  // allocate an initial array of size 1
  size_t capacity = 1;
  *n = 0;
  // we can't use sizeof location; it's frosted glass, we can't see inside the struct
  // the way we've defined it
  // Location IS AN OPAQUE STRUCT
  location *locs = malloc(capacity * sizeof(location));
  
  double lat, lon;
  while (scanf("%lf %lf", &lat, &lon) == 2)
    {
      if (*n == capacity)
	{
	  // array was full -- allocate a larger array
	  location *bigger = malloc(capacity * 2 * sizeof(location));
	  if (bigger != NULL)
	    {
	      // copy into larger array
	      for (size_t i = 0; i < *n; i++)
		{
		  bigger[i] = locs[i];
		}

	      // free old, too-small array
	      free(locs);

	      // remember new, larger array and its capacity
	      locs = bigger;
	      capacity *= 2;
	    }
	}

      if (*n < capacity)
    	{
    	  // there is room; add location read
        locs[*n] = location_create(*lat, *lon);
    	  (*n)++;
    	}
    }

  // validate all stored locations by searching for invalid one
  size_t i = 0;
  while (i < *n && location_validate(&locs[i]))
    {
      i++;
    }
  
  if (i < *n)
    {
      // found an invalid location: free all stored locations and fail
      free(locs);
      *n = 0;
      return NULL;
    }
  else
    {
      return locs;
    }
}
