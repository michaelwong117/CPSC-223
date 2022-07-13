#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "usdot.h"
#include "string_util.h"

#define USDOT_CODE_LENGTH 3

bool read_segment(FILE *stream, int *id, int *seq, int *tot, char *origin, char *destination)
{
  // zero everything out so we can check later if they were read correctly
  *id = 0;
  *seq = 0;
  *tot = 0;
  origin[0] = '\0';
  destination[0] = '\0';

  // no line in the database is > 80 characters, but use read_line to be safe
  char line[81];
  read_line(line, 80);

  // check that we read enough to at least get the id
  int len = strlen(line);
  
  if (strlen(line) > 2)
    {
      // skip the 1st two digits in the id (they're always 20 in our data)
      // (if we want all the digits then we need to use a long for the ids
      // and hence our map need to work with longs as keys)
      *id = atoi(line + 2);

      // look for spaces in the input, and pluck out the parts before
      // the 2nd, 3rd, 5th, and 6th spaces, which are the sequence number,
      // number of records in the sequence, origin, and destination
      // respectively
      int space = 0;
      char *last = line;
      for (int i = 0; i < len; i++)
	{
	  if (line[i] == ' ')
	    {
	      // found a space...count it and replace with a null character
	      // so we can treat from after the last space to here as a string
	      space++;
	      line[i] = '\0';
	      if (space == 2)
		{
		  *seq = atoi(last);
		}
	      else if (space == 3)
		{
		  *tot = atoi(last);
		}
	      else if (space == 5)
		{
		  if (strlen(last) == USDOT_CODE_LENGTH)
		    {
		      strcpy(origin, last);
		    }
		}
	      else if (space == 6)
		{
		  if (strlen(last) == USDOT_CODE_LENGTH)
		    {
		      strcpy(destination, last);
		    }
		}

	      // remember location of this space to use as the beginning
	      // of the string ended by the next space we found
	      last = line + i + 1;
	    }
	}
    }

  // return true if we were able to read everything
  return (*id != 0 && *seq != 0 && *tot != 0 && strlen(origin) > 0 && strlen(destination) > 0);
}
