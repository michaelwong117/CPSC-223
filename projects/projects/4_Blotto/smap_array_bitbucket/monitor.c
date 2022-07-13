#include <stdio.h>
#include <stdlib.h>

#include "smap.h"


/**
 * A temperature/relative humidity reading from a weather station.
 */
typedef struct obs
{
  double temp;
  double rh;
} observation;


/**
 * A hash function for strings.
 *
 * @param s a pointer to a string, non-NULL
 * @return the hash code for s
 */
size_t really_bad_hash(const char *s);


/**
 * Saves the given reading in the map under the given station id.
 *
 * @param m a pointer to a map, non-NULL
 * @param id a pointer to a string, non-NULL
 * @param temp a double
 * @param rh a double in the range 0.0 to 100.0 (inclusive)
 */
void record_reading(smap *m, char *id, double temp, double rh);


/**
 * Frees the value in the given map entry.
 *
 * @param key ignored
 * @param value a pointer to a value
 */
void free_value(const char *key, void *value, void *arg);


/**
 * Prints a temperature/relative humidity reading to standard output.
 *
 * @param key a pointer to a string, non-NULL
 * @param value a pointer to an observation, non-NULL
 * @param arg ignored
 */
void print_current_observation(const char *key, void *value, void *arg);


int main(int argc, char **argv)
{
  smap *locations = smap_create(really_bad_hash);
  smap_put(locations, "ITH123", "Master Bedroom");
  smap_put(locations, "OTH321", "Deck");
  smap_put(locations, "ITH999", "Sunroom");
  smap_put(locations, "DIS444", "Kitchen");
    
  smap *current_obs = smap_create(really_bad_hash);

  record_reading(current_obs, "ITH123", 68.8, 46.5);
  record_reading(current_obs, "OTH321", 54.1, 40.9);
  record_reading(current_obs, "ITH999", 79.2, 51.2);
  record_reading(current_obs, "ITH123", 68.7, 46.8);
  record_reading(current_obs, "ITH#@3", 164.8, -9.5);

  smap_for_each(locations, print_current_observation, current_obs);

  smap_destroy(locations);
  smap_for_each(current_obs, free_value, NULL);
  smap_destroy(current_obs);
}


size_t really_bad_hash(const char *s)
{
  int sum = 0;
  while (*s != '\0')
    {
      sum += *s;
      s++;
    }
  return sum;
}


void record_reading(smap *m, char *id, double temp, double rh)
{
  if (smap_contains_key(m, id))
    {
      observation *obs = smap_get(m, id);
      obs->temp = temp;
      obs->rh = rh;
    }
  else
    {
      observation *obs = malloc(sizeof(observation));
      if (obs != NULL)
	{
	  obs->temp = temp;
	  obs->rh = rh;
	  smap_put(m, id, obs);
	}
    }
}


void free_value(const char *key, void *value, void *arg)
{
  free(value);
}


void print_current_observation(const char *key, void *value, void *arg)
{
  smap *current_obs = arg;

  if (smap_contains_key(current_obs, key))
    {
      observation *obs = smap_get(current_obs, key);
      printf("%s %f %f\n", key, obs->temp, obs->rh);
    }
}

