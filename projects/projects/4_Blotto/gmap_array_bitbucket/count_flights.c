#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "usdot.h"
#include "string_key.h"
#include "gmap.h"


void free_value(const void *key, void *value, void *);
void print_entry(const void *key, void *value, void *);

/**
 * Adds one to the value associated with the given string in the given map,
 * or sets the value to 1 if the string was not already present as a key.
 *
 * @param c a pointer to a map, non-NULL
 * @param airport a pointer to a string, non-NULL
 */
void update_count(gmap *c, const char *airport);

#define CODE_LENGTH 3

int main(int argc, char **argv)
{
  gmap *count = gmap_create(duplicate, compare_keys, hash29, free);

  int id, seq, tot;
  char origin[CODE_LENGTH + 1];
  char destination[CODE_LENGTH + 1];
  while (read_segment(stdin, &id, &seq, &tot, origin, destination))
    {
      update_count(count, origin);
      update_count(count, destination);
    }

  gmap_for_each(count, print_entry, NULL);

  // free allocated memory
  gmap_for_each(count, free_value, NULL);
  gmap_destroy(count);
}

/**
 * Prints the given map entry to standard output.
 */
void print_entry(const void *key, void *value, void *arg)
{
  printf("%s=%d\n", (const char *)key, *((int *)value));
}

/**
 * Frees the value in the given map entry.
 */
void free_value(const void *key, void *value, void *arg)
{
  free(value);
}


/**
 * Updates the count in the given map for the given string.
 *
 * @param m a map, non-NULL
 * @param s a key, non-NULL
 */
void update_count(gmap *m, const char *airport)
{
  if (m == NULL || airport == NULL)
    {
      return;
    }
  
  if (!gmap_contains_key(m, airport))
    {
      // key is not present; add it with new value of 1
      size_t *c = malloc(sizeof(size_t));
      *c = 1;
      gmap_put(m, airport, c);
    }
  else
    {
      size_t *c = gmap_get(m, airport);
      (*c)++;
    }
}
