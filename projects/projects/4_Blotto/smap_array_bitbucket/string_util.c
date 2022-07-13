#include <stdio.h>
#include <ctype.h>

#include "string_util.h"

void read_line(char s[], int max)
{
  int count = 0; // number of chars read
  int ch;
  while ((ch = getchar()) != EOF && ch != '\n')
    {
      if (count < max)
	{
	  s[count] = ch;
	}
      count++;
    }
  s[(count > max ? max : count)] = '\0';
}

size_t really_bad_string_hash(const char *key)
{
  if (key == NULL)
    {
      return 0;
    }
  else
    {
      // key hashes to ASCII value of first character
      // (this does *not* achieve the goal of spreading all possible keys
      // uniformly and pseudo-randomly across the range of an int)
      // but the current smap implementation does't even use hashing, so whatevs
      return *key;
    }
}
