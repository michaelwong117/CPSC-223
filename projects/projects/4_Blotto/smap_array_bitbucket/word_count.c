/**
 * Counts words that appear on the command line (up to "-contains") and checks whether
 * words appeared (after "-contains").
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smap.h"

/**
 * Frees the value in the given map entry.
 *
 * @param key ignored
 * @param value a pointer to a value
 */
void free_value(const char *key, void *value, void *arg);


/**
 * Prints the given entry to standard output.
 *
 * @param key a pointer to a string, non-NULL
 * @param value a pointer to an integer, non-NULL
 * @param arg ignored
 */
void print_entry(const char *key, void *value, void *arg);


/**
 * Prints the given entry to standard output if the key starts with the given
 * character.  Otherwise, there is no effect.
 *
 * @param key a pointer to a string, non-NULL
 * @param value a pointer to an integer, non-NULL
 * @param arg a pointer to a character, non-NULL
 */
void print_if_initial_is(const char *key, void *value, void *arg);


/**
 * A hash function for strings.
 *
 * @param s a pointer to a string, non-NULL
 * @return the hash code for s
 */
size_t really_bad_string_hash(const char *key);


int main(int argc, char **argv)
{
  smap *counts = smap_create(really_bad_string_hash);
  if (counts == NULL)
    {
      return 1;
    }

  int a = 1;
  while (a < argc && strcmp(argv[a], "-contains") != 0)
    {
      // check whether the word was already present
      if (!smap_contains_key(counts, argv[a]))
	{
	  // word was not present, add entry with value 1
	  // (values is our smap implementation are pointers to integers,
	  // so we have to dynamically allocate an int and initialize it
	  // to 1 [the reason is so we can use the same smap for either
	  // single integers or arrays of integers])
	  
	  int *one = malloc(sizeof(int));
	  *one = 1;

	  if (smap_put(counts, argv[a], one) == smap_error)
	    {
	      fprintf(stderr, "%s: error allocating entry\n", argv[0]);
	      free(one);
	    }
	}
      else
	{
	  // word was already present; get the old value, add one to it,
	  // and put it back to replace the old value
	  int *old_count = smap_get(counts, argv[a]);
	  *old_count = *old_count + 1;

	  // (but note that since we're using pointers this is not
	  // actually necessary; we include it to illustrate a more
	  // typical case of updating a value stored in a map)
	  smap_put(counts, argv[a], old_count);
	}
      a++;
    }

  printf("=== Counts ===\n");
  smap_for_each(counts, print_entry, NULL);

  printf("=== Counts of the words that begin with 'C' ===\n");
  char *start = "C"; // a string, but print_if_c only looks at the first character
  smap_for_each(counts, print_if_initial_is, start);
    
  printf("=== Contains ===\n");
  // advance over "-contains"
  a++;

  while (a < argc)
    {
      printf("%s %s\n", argv[a], smap_contains_key(counts, argv[a]) ? "true" : "false");
      a++;
    }

  // free all the dynamically allocated ints we put in the map as values
  smap_for_each(counts, free_value, NULL);
  
  // destroy the map
  smap_destroy(counts);

  return 0;
}


void free_value(const char *key, void *value, void *arg)
{
  free(value);
}


void print_entry(const char *key, void *value, void *arg)
{
  if (key != NULL && value != NULL)
    {
      printf("%s = %d\n", key, *((int *)value));
    }
}


void print_if_initial_is(const char *key, void *value, void *arg)
{
  if (key != NULL && value != NULL && arg != NULL && *key == *((char *)arg))
    {
      printf("%s = %d\n", key, *((int *)value));
    }
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
