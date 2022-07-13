#ifndef __OUTPUT_H__
#define __OUTPUT_H__

/**
 * Writes the given interval to standard output.  The interval is given by sample index and
 * the output in converted to seconds by dividing by the given rate.
 *
 * @param start an integer
 * @param end an integer
 * @param rate a non-zero integer
 */
void output_interval(int start, int end, int rate);

#endif

