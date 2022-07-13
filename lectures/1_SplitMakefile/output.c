#include <stdio.h>

#include "output.h"

void output_interval(int start, int end, int rate)
{
  printf("[%lf-%lf]\n", (double)start / rate, (double)end / rate);
}
