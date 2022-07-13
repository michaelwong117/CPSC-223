#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "usdot.h"
#include "string_util.h"
#include "smap.h"


/**
 * Frees the value in the given map entry.
 *
 * @param key ignored
 * @param value a pointer to a value
 */
void free_value(const char *key, void *value, void *);


/**
 * Prints the given entry to standard output.
 *
 * @param key a pointer to a string, non-NULL
 * @param value a pointer to an integer, non-NULL
 */
void print_entry(const char *key, void *value, void *);


/**
 * Updates the count in the given map for the given string.
 *
 * @param m a map, non-NULL
 * @param s a key, non-NULL
 */
void update_count(smap *c, const char *s);


#define CODE_LENGTH 3


int main(int argc, char **argv)
{
  smap *count = smap_create(really_bad_string_hash);

  int id, seq, tot;
  char origin[CODE_LENGTH + 1];
  char destination[CODE_LENGTH + 1];
  while (read_segment(stdin, &id, &seq, &tot, origin, destination))
    {
      update_count(count, origin);
      update_count(count, destination);
    }

  smap_for_each(count, print_entry, NULL);

  // free allocated memory
  smap_for_each(count, free_value, NULL);
  smap_destroy(count);
}


/**
 * Prints the given map entry to standard output.
 */
void print_entry(const char *key, void *value, void *arg)
{
  printf("%s=%d\n", key, *((int *)value));
}


/**
 * Frees the value in the given map entry.
 */
void free_value(const char *key, void *value, void *arg)
{
  free(value);
}


void update_count(smap *m, const char *s)
{
  if (m == NULL || s == NULL)
    {
      return;
    }
  
  if (!smap_contains_key(m, s))
    {
      // key is not present; add it with new value of 1
      int *c = malloc(sizeof(int));
      *c = 1;
      smap_put(m, s, c);
    }
  else
    {
      int *c = smap_get(m, s);
      (*c)++;
    }
}
