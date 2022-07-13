#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "location.h"
#include "location_list.h"

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
 * Reads locations from standard input and returns them in a list.
 * Standard input should contain one line per location, with a latitude
 * and longitude on each line, separated by whitespace.  This function
 * will read until no more locations can be read.  If there is a memory
 * allocation failure then the number of locations in the list may be less
 * than the number of locations read.  If any latitude or longitude is
 * invalid, then locations are still read until non-numeric data is
 * encountered, but the returned list is NULL.  It is the caller's
 * responsibility to destroy the list.
 *
 * @return a pointer to a list of locations
 */
location_list *read_locations();


/**
 * Prints a location
 *
 * @param loc a pointer to a location, non-NULL
 */
void print_location(const location *loc);


/**
 * Compares two locations based on their longitudes, breaking ties
 * by latitude.
 *
 * @param loc1 a pointer to a location
 * @param loc2 a pointer to a location
 * @param arg unused
 */
int compare_lon(const location *loc1, const location *loc2, const void *arg);

/**
 * Compares two locations based on their latitudes, breaking ties
 * by longitude.
 *
 * @param a1 a pointer to a pointer to a location
 * @param a2 a pointer to a pointer to a location
 */
int compare_lat(const void *a1, const void *a2);

/**
 * Compares two locations based on their distance from a third base location.
 * by latitude.
 *
 * @param loc1 a pointer to a location
 * @param loc2 a pointer to a location
 * @param arg a pointer to a location
 */
int compare_by_distance_from_base(const location *loc1, const location *loc2, const void *arg);

/**
 * Computes the distance of a closed path given the latitudes and longitudes
 * of the points on the path.  The coordinates are read from standard input
 * and assumed to be in degrees.  The output is the total distance along
 * the path created by visiting the points in order of increasing longitude
 * and returning to the westernmost.
 *
 * @version 0.3 2021-09-27
 */
int main()
{
  // read in latitude and longitudes
  location_list *stops = read_locations();
  if (stops == NULL)
    {
      return 1;
    }

  // We can't pass location_compare directly since it takes
  // two pointers to locations, and our comparison functions must take
  // two pointers to locations AND an extra argument.
  // The compare_lon function calls location_compare and ignores the
  // extra argument.
  location_list_sort(stops, compare_lon, NULL);
  
  double total_distance = location_list_closed_distance(stops);
  printf("BY LONGITUDE\ndistance %lf miles\n", total_distance);
  location_list_for_each(stops, print_location);

  // sort again but based on distance from DL 220
  location *base = location_create(41.312510, -72.924485);
  location_list_sort(stops, compare_by_distance_from_base, base);
  total_distance = location_list_closed_distance(stops);
  printf("BY DISTANCE FROM DL220\ndistance %lf miles\n", total_distance);
  location_list_for_each(stops, print_location);

  // put everything into an array (O(n^2) since we have to use get because
  // we don't have a for_each that takes an extra argument (which could
  // have been the list to add things to)
  size_t len = location_list_size(stops);
  location *arr[len];
  
  // what we could do if we had such a for_each:
  // typedef struct _adder_info { location **arr, int index } adder_info;
  // adder_info info {arr, 0}
  // location_list_for_each(stops, add_to_array, &info);
  // add_to_array would do {info->arr[info->index] = elt; info->index++}

  // what we have to do instead
  for (size_t i = 0; i < len; i++)
    {
      arr[i] = location_list_get(stops, i);
    }
  qsort(arr, len, sizeof(*arr), compare_lat);
  printf("AFTER qsort\n");
  for (size_t i = 0; i < len; i++)
    {
      printf("%.3lf %.3lf\n",
	     location_get_latitude(arr[i]),
	     location_get_longitude(arr[i]));
    }
	
  
  location_destroy(base);
  location_list_destroy(stops);
}


location_list *read_locations()
{
  location_list *result = location_list_create();
  if (result == NULL)
    {
      return NULL;
    }
  
  double lat, lon;
  while (scanf("%lf %lf", &lat, &lon) == 2)
    {
      location *curr = location_create(lat, lon);
      location_list_add(result, curr);
    }

  // validate all stored locations by searching for invalid one
  size_t i = 0;
  size_t n = location_list_size(result);
  while (i < n && location_validate(location_list_get(result, i)))
    {
      i++;
    }
  
  if (i < n)
    {
      // found an invalid location: free all stored locations and fail
      location_list_destroy(result);
      result = NULL;
    }

  return result;
}

void print_location(const location *loc)
{
  printf("%.3lf %.3lf\n", location_get_latitude(loc), location_get_longitude(loc));
}

int compare_lon(const location *loc1, const location *loc2, const void *arg)
{
  return location_compare(loc1, loc2);
}

int compare_lat(const void *a1, const void *a2)
{
  // a1 and a2 are pointers to the array elements to sort; those array
  // elements are themselved pointers to locations, so a1 and a2 are
  // really pointers to pointers to locations and we dereference once to
  // the the pointers to the locations we're trying to compare
  // Why two consts?  because we're promising to not change the pointers
  // in the array and the locations at the ends of those pointers.
  // The first const says we're not changing the locations -- **loc is what's
  // after const, and that's what that const says we're not chanfing.
  // The second const has *loc1 after it to say we're not changing what's
  // at the end of the loc1/a1 pointer, which is the pointer in the array.
  // We need the second one here because const void *a1 is a promise not
  // to change whatever in the array is pointed to by a1.
  const location * const *loc1 = a1;
  const location * const *loc2 = a2;

  double diff = location_get_latitude(*loc1) - location_get_latitude(*loc2);
  if (diff < 0)
    {
      return -1;
    }
  else if (diff == 0)
    {
      // use location_compare to break ties on longitude
      return location_compare(*loc1, *loc2);
    }
  else
    {
      return 1;
    }
}

int compare_by_distance_from_base(const location *loc1, const location *loc2, const void *arg)
{
  const location *base = arg;
  double d1 = location_distance(loc1, base);
  double d2 = location_distance(loc2, base);

  if (d1 < d2)
    {
      return -1;
    }
  else if (d1 > d2)
    {
      return 1;
    }
  else
    {
      // break ties based on longitude, then latitude
      return location_compare(loc1, loc2);
    }
}
