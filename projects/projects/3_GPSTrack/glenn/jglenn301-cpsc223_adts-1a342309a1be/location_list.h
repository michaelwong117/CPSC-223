#ifndef __LOCATION_LIST_H__
#define __LOCATION_LIST_H__

#include <stdlib.h>
#include <stdbool.h>

#include "location.h"

typedef struct _loc_list location_list;

/**
 * Creates an empty list of locations.  The return value is NULL if there
 * was an allocation error creating the new list.
 */
location_list *location_list_create();

/**
 * Returns the size of the given list.
 *
 * @param l a pointer to a list of locations
 */
size_t location_list_size(const location_list *l);


/**
 * Returns the location at the given location in this list.  The list
 * retains ownership of the returned item.
 *
 * @param l a pointer to a list of locations
 * @param i an index into that list
 * @return the location at that position
 */
location *location_list_get(const location_list *l, size_t i);


/**
 * Adds the given location to the end of the given list.  Ownership
 * of the location is transferred to the list.
 *
 * @param l a pointer to a list of locations
 * @param loc a pointer to a location
 * @return true if and only if the location was sucessfully added
 */
bool location_list_add(location_list *l, location *loc);


/**
 * Sorts the locations in the given list according to the given comparison
 * function and extra argument.
 *
 * @param l a pointer to a list of locations
 * @param comp a pointer to a comparison function that takes two
 * non-NULL pointers to locations and an extra argument
 * @param arg a pointer to an extra argument
 */
void location_list_sort(location_list *l, int (*comp)(const location *, const location *, const void *), const void *arg);


/**
 * Returns the total distance of the given list of cities, considered as
 * a closed tour that visits cities in the order given and then returns to
 * the first.
 *
 * @param l a pointer to a list of locations
 */
double location_list_closed_distance(const location_list *l);


/**
 * Destroys the given list.  There is no effect if the argument is NULL.
 *
 * @param l a pointer to a list of locations
 */
void location_list_destroy(location_list *l);


/**
 * Passes each location on list to the given function in the order the
 * locations appear in the list.
 *
 * @param l a pointer to a list of locations, non-NULL
 * @param f a pointer to a function, non-NULL
 */
void location_list_for_each(const location_list *l, void (*f)(const location *loc));

#endif
