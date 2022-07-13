#include "smap.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

char *smap_error = "error";


/**
 * Nodes to implement chains as singly-linked lists.
 */
typedef struct _smap_node
{
  char *key;
  void *value;
  struct _smap_node *next;
} smap_node;


/**
 * Meta-data struct for nodes.
 */
struct _smap
{
  smap_node **table;        // an array of pointers to heads of chains
  size_t size;         // the number of (key, value) pairs in the map
  size_t num_chains;   // the number of chains in the hash table
  
  size_t (*hash)(const char *);
};


#define SMAP_INITIAL_CAPACITY 100


/**
 * Compute the index of the slot in which the given key would be
 * found in the given hash table.
 *
 * @param key a pointer to a string, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 */
size_t smap_compute_index(const char *key, size_t (*hash)(const char *), size_t num_chains);


/**
 * Resizes the hash table in the given map to have at least the given number
 * of slots.  The current implementation does nothing.
 *
 * @param m a pointer to a map, non-NULL
 * @param n a positive integer
 */
void smap_embiggen(smap *m, size_t n);


/**
 * Adds the given node to the given hash table using the given hash function.
 *
 * @param table a pointer to an array of num_chains heads of chains, non-NULL
 * @param n a pointer to a node containing a non-NULL key and a value, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 */
void smap_table_add(smap_node **table, smap_node *n, size_t (*hash)(const char *), size_t num_chains);


/**
 * Returns the node in the given chained hash table containing the given
 * key, or NULL if there is no such node.
 *
 * @param table a pointer to an array of num_chains heads of chains, non-NULL
 * @param key a pointer to a string, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 * @return a pointer to a node containing key, or NULL
 */
smap_node *smap_table_find_key(smap_node **table, const char *key, size_t (*hash)(const char *), size_t num_chains);


smap *smap_create(size_t (*h)(const char *s))
{
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(smap_node *) * SMAP_INITIAL_CAPACITY);
      result->num_chains = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
      for (size_t i = 0; i < result->num_chains; i++)
	{
	  // all chains are initially empty
	  result->table[i] = NULL;
	}
    }
  return result;
}


size_t smap_size(const smap *m)
{
  if (m == NULL)
    {
      return 0;
    }
  
  return m->size;
}


smap_node *smap_table_find_key(smap_node **table, const char *key, size_t (*hash)(const char *), size_t num_chains)
{
  // determine which chain to search from hash function
  size_t i = smap_compute_index(key, hash, num_chains);

  // sequential search of the chain
  smap_node *curr = table[i];
  while (curr != NULL && strcmp(curr->key, key) != 0)
    {
      curr = curr->next;
    }
  return curr;
}


void *smap_put(smap *m, const char *key, void *value)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  smap_node *n = smap_table_find_key(m->table, key, m->hash, m->num_chains);
  if (n != NULL)
    {
      // key already present
      void *old_value = n->value;
      n->value = value;
      return old_value;
    }
  else
    {
      // make space for a copy of the key
      char *copy = malloc(strlen(key) + 1);
      
      if (copy != NULL)
	{
	  // copy contents of key
	  strcpy(copy, key);
	  
	  // check if load factor is too high
	  if (m->size >= m->num_chains)
	    {
	      // add chains and rehash
	      smap_embiggen(m, m->num_chains * 2);
	    }
	      
	  // add to table
	  smap_node *n = malloc(sizeof(smap_node));
	  if (n != NULL)
	    {
	      n->key = copy;
	      n->value = value;
	      smap_table_add(m->table, n, m->hash, m->num_chains);
	      m->size++;
	      return NULL;
	    }
	  else
	    {
	      free(copy);
	      return smap_error;
	    }
	}
      else
	{
	  return smap_error;
	}
    }
}


void smap_table_add(smap_node **table, smap_node *n, size_t (*hash)(const char *), size_t num_chains)
{
  // get index of chain to add in
  size_t i = smap_compute_index(n->key, hash, num_chains);

  // add to front of chain
  n->next = table[i];
  table[i] = n;
}


void smap_embiggen(smap *m, size_t n)
{
  // this implementation does nothing; load factor will grow without bound!
  // NOOOOOOoooooooo! [quiet sobbing]
}

size_t smap_compute_index(const char *key, size_t (*hash)(const char *), size_t num_chains)
{
  // hash and take remainder
  return hash(key) % num_chains;
}


bool smap_contains_key(const smap *m, const char *key)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  return smap_table_find_key(m->table, key, m->hash, m->num_chains) != NULL;
}


void *smap_get(smap *m, const char *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  // find the node containing the key
  smap_node *n = smap_table_find_key(m->table, key, m->hash, m->num_chains);
  if (n != NULL)
    {
      // return the value in that node
      return n->value;
    }
  else
    {
      // not found
      return NULL;
    }
}


void *smap_remove(smap *m, const char *key)
{
  // remove not implemented
  return NULL;
}


/**
 * A location in an array where a key can be stored.  The location is
 * represented by a (array, index) pair.
 */
typedef struct _smap_store_location
{
  const char **arr;
  size_t index;
} smap_store_location;



/**
 * Stores the given key in the given location and increments the location
 * by one element.
 *
 * @param key a pointer to a key, non-NULL
 * @param value ignored
 * @param arg a pointer to a smap_store_location
 */
void smap_store_key_in_array(const char *key, void *value, void *arg)
{
  smap_store_location *where = arg;
  where->arr[where->index] = key;
  where->index++;
}


const char **smap_keys(smap *m)
{
  const char **keys = malloc(sizeof(*keys) * m->size);

  if (keys != NULL)
    {
      smap_store_location loc = {keys, 0};
      smap_for_each(m, smap_store_key_in_array, &loc);
    }

  return keys;
}



void smap_for_each(smap *m, void (*f)(const char *, void *, void *), void *arg)
{
  if (m == NULL || f == NULL)
    {
      return;
    }

  for (size_t chain = 0; chain < m->num_chains; chain++)
    {
      smap_node *curr = m->table[chain];
      while (curr != NULL)
	{
	  f(curr->key, curr->value, arg);
	  curr = curr->next;
	}
    }
}  


void smap_destroy(smap *m)
{
  if (m == NULL)
    {
      return;
    }

  // iterate over all chains
  for (size_t i = 0; i < m->num_chains; i++)
    {
      // iterate over all nodes in chain
      smap_node *curr = m->table[i];
      while (curr != NULL)
	{
	  // free key
	  free(curr->key);

	  // remember where to go next
	  smap_node *next = curr->next;

	  // free node
	  free(curr);

	  // go to next node
	  curr = next;
	}
    }

  free(m->table);
  free(m);
}
