#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <stdbool.h>

typedef struct _location location;

// Have all the declarations of the functions in a header file in order to call them
// The functions themselves are written in the corresponding .c file

// Can always create a pointer to something because pointers are all the same size

location *location_create(double lat, double lon);

/**
 * Returns the distance between the two locations on the Earth's surface,
 * assuming a spherical earth with radius 6378.1 km.
 *
 * @param l1 a location
 * @param l2 a location
 * @return the distance between those points
 */
double location_distance(const location *l1, const location *l2);
double location_distance_spherical(const location *l1, const location *l2);
double location_distance_oblate(const location *l1, const location *l2);

/**
 * Determines if the given location is valid.  A location is valid if the
 * latitude is between -90 and 90 degrees.
 *
 * @param l a pointer to a location, non-NULL
 * @return true if the location is valid, false otherwise
 */
bool location_validate(const location *l);

#endif
