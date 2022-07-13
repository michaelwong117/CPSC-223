#ifndef __USDOT_H__
#define __USDOT_H__

#include <stdio.h>
#include <stdbool.h>

/**
 * Reads a record from the US DOT coupons database into the given
 * (simulated) reference parameters.
 *
 * @param stream the stream to read from
 * @param id a pointer to an int, non-NULL
 * @param tot a pointer to an int, non-NULL
 * @param seq a pointer to an int, non-NULL
 * @param origin a pointer to an array of 4 characters, non-NULL
 * @param destination a pointer to an array of 4 characters, non-NULL
 */
bool read_segment(FILE *stream, int *id, int *seq, int *tot, char *origin, char *destination);

#endif
