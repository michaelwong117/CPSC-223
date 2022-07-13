#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <stdbool.h>

typedef struct _location location;

/**
 * Creates a location with the given latitude and longitude.  If the
 * latitude is invalid or there is a memory allocation errir then the
 * return value is NULL.  It is the caller's responsibility to
 * eventually destroy the returned location if it is not NULL.  .
 * location *location_create(double lat, double lon);
 * 
 * @param lat a value between -90 and 90, inclusive
 * @param lon a longitude
 */
location *location_create(double lat, double lon);

/**
 * Destroys the given location.
 *
 * @param loc a pointer to a location, non-NULL
 */
void location_destroy(location *loc);

/**
 * Compares two locations by longitude, breaking ties by latitude.
 * The return value is positive if the first location is greater
 * than the second (has a greater longitude), negative if the first
 * is lesser, and 0 if they are equal.
 * 
 * @param l1 a pointer to a location
 * @param l2 a pointer to a location
 * @return an integer
 */
int location_compare(const location *l1, const location *l2);

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
