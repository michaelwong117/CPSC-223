#include "gmap.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

char *gmap_error = "error";

/**
 * Nodes to implement chains as singly-linked lists.
 */
typedef struct _gmap_node
{
  void *key;
  void *value;
  struct _gmap_node *next;
} gmap_node;


/**
 * Meta-data struct for nodes.
 */
struct gmap
{
  gmap_node **table;        // an array of pointers to heads of chains
  size_t size;         // the number of (key, value) pairs in the map
  size_t num_chains;   // the number of chains in the hash table
  size_t (*hash)(const void *);
  int (*compare)(const void *, const void *);
  void *(*copy)(const void *);
  void (*free)(void *);

};

#define GMAP_INITIAL_CAPACITY 10


/**
 * Creates an empty map that uses the given hash function.
 *
 * @param cp a function that take a pointer to a key and returns a pointer to a deep copy of that key
 * @param comp a pointer to a function that takes two keys and returns the result of comparing them,
 * with return value as for strcmp
 * @param h a pointer to a function that takes a pointer to a key and returns its hash code
 * @param f a pointer to a function that takes a pointer to a copy of a key make by cp and frees it
 * @return a pointer to the new map or NULL if it could not be created;
 * it is the caller's responsibility to destroy the map
 */
gmap *gmap_create(void *(*cp)(const void *), int (*comp)(const void *, const void *), size_t (*h)(const void *s), void (*f)(void *))
{
  if (h == NULL || cp == NULL || comp == NULL || f == NULL)
    {
      // one of the required functions was missing
      return NULL;
    }
  
  gmap *result = malloc(sizeof(gmap));

	if (result != NULL)
  	{
		// remember the functions used to manipulate the keys
		result->copy = cp;
		result->compare = comp;
		result->hash = h;
		result->free = f;

		// initialize the table
		result->table = malloc(sizeof(gmap_node *) * GMAP_INITIAL_CAPACITY);
		result->size = 0;
		result->num_chains = (result->table != NULL ? GMAP_INITIAL_CAPACITY : 0);
    	for (size_t i = 0; i < result->num_chains; i++)
		{
			// all chains are initially empty
			result->table[i] = NULL;
		}

	}

	return result;
}


size_t gmap_size(const gmap *m)
{
  if (m == NULL)
    {
      return 0;
    }
  
  return m->size;
}

size_t gmap_compute_index(const void *key, size_t (*hash)(const void *), size_t num_chains)
{
  // hash and take remainder
  return hash(key) % num_chains;
}


gmap_node *gmap_table_find_key(gmap_node **table, const void *key, size_t (*hash)(const void *), int (*compare)(const void *, const void *), size_t num_chains)
{
	if(table == NULL || key == NULL) return NULL;
  // determine which chain to search from hash function
  size_t i = gmap_compute_index(key, hash, num_chains);

  // sequential search of the chain
  gmap_node *curr = table[i];
  while (curr != NULL && compare(curr->key, key) != 0)
    {
      curr = curr->next;
    }
  return curr;
}


/* temp functions to test */


void gmap_table_add(gmap_node **table, gmap_node *n, size_t (*hash)(const void *), size_t num_chains)
{
	if(table == NULL || n == NULL) return;
  // get index of chain to add in
  size_t i = gmap_compute_index(n->key, hash, num_chains);

  // add to front of chain
  n->next = table[i];
  table[i] = n;
}

/**
 * Adds a copy of the given key with value to this map.
 * If the key is already present then the old value is replaced.
 * The caller retains ownership of the value.  Note that if replacing the old value would
 * cause resource leaks, then the caller should retrieve the old value with get and release
 * its resources before calling put to replace it.
 *
 * @param m a map, non-NULL
 * @param key a pointer to a key, non-NULL
 * @param value a pointer to an integer
 * @return true if the put was successful, false otherwise
 */

bool gmap_put(gmap *m, const void *key, void *value)
{
	if (m == NULL || key == NULL)
	{
	  return false;
	}

	gmap_node *n = gmap_table_find_key(m->table, key, m->hash, m->compare, m->num_chains);

	if (n != NULL)
	{
		// key already present
		void *old_value = n->value; // it's not this, so it works **old_value = &(n->value)
		n->value = value;
		free(old_value);
		return true;
	}
	else
	{
		// printf("creating key %s with value %p\n", key, value);
		// copy the key
		void *copy = m->copy(key);
	  
		if (copy != NULL)
		{

		  // check if load factor is too high
		  if (m->size >= m->num_chains)
		    {
		      // add chains and rehash
		      gmap_embiggen(m, m->num_chains * 2);
		    }
		      
		  // add to table
		  gmap_node *n = malloc(sizeof(gmap_node));
		  if (n != NULL)
		    {
		      n->key = copy;
		      n->value = value;
		      // printf("creating key %s with value %p\n", key, value);
		      gmap_table_add(m->table, n, m->hash, m->num_chains);
		      m->size++;
		      return true;
		    }
		  else
		    {
		      free(copy);
		      return false;
		    }
		}
		else
		{
	       return false;
		}
	}
}


/**
 * Destroys the given map.  There is no effect if the given pointer is NULL.
 *
 * @param m a pointer to a map, or NULL
 */
void gmap_destroy(gmap *m)
{
	if (m == NULL)
	{
	  return;
	}

  // iterate over all chains
  for (size_t i = 0; i < m->num_chains; i++)
    {
      // iterate over all nodes in chain
      gmap_node *curr = m->table[i];
      while (curr != NULL)
	  {
	  // free key
	  free(curr->key);

	  // remember where to go next
	  gmap_node *next = curr->next;

	  // free node
	  free(curr);

	  // go to next node
	  curr = next;
	  }
    }

  free(m->table);
  free(m);
}

/**
 * Returns the value associated with the given key in this map.
 * If the key is not present in this map then the returned value is
 * NULL.  The pointer returned is the original pointer passed to gmap_put,
 * and it remains the responsibility of whatever called gmap_put to
 * release the value it points to (no ownership transfer results from
 * gmap_get).
 *
 * @param m a pointer to a map, non-NULL
 * @param key a pointer to a key, non-NULL
 * @return a pointer to the assocated value, or NULL if they key is not present
 */
void *gmap_get(gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  // find the node containing the key
  gmap_node *n = gmap_table_find_key(m->table, key, m->hash, m->compare, m->num_chains);
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

bool gmap_contains_key(const gmap *m, const void *key)
{
  if (m == NULL || key == NULL)
    {
      return false;
    }

  return gmap_table_find_key(m->table, key, m->hash, m->compare, m->num_chains) != NULL;
}


/**
 * Removes the given key and its associated value from the given map if
 * the key is present.  The return value is NULL and there is no effect
 * on the map if the key is not present.  They copy of the key held by
 * the map is destroyed.  It is the caller's responsibility to free the
 * returned value if necessary.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a key, non-NULL
 * @return the value associated with the removed key, or NULL
 */
void *gmap_remove(gmap *m, const void *key)
{
	if (m == NULL || key == NULL)
	{
	  return NULL;
	}

	gmap_node *n = gmap_table_find_key(m->table, key, m->hash, m->compare, m->num_chains);

	if (n != NULL)
	{
		// key present, remove and return the value

		void *return_value = n->value; // storing the key value

		// determine which chain to search from hash function
		size_t i = gmap_compute_index(key, m->hash, m->num_chains);

		// sequential search of the chain

		gmap_node *curr = m->table[i];
		gmap_node *prev = NULL;

		while (curr != NULL && m->compare(curr->key, key) != 0)
		{
		  	prev = curr;
		    curr = curr->next;
		}
		// if there is no node after the one we're removing in the chain
		if (curr->next == NULL)
		{
			if (prev == NULL) // there is no node before either
			{
				free(curr->key);
				free(curr);
				m->table[i] = NULL;
			}
			else if (prev != NULL)
			{
				prev->next = NULL;
				free(curr->key);
				free(curr);

				// wrong because the earliest would already be at 
				// the front of the chain: table[i]
				// table[i] = prev; 
			}
		}
		// if there is a node after the one we're removing in the chain
		else if (curr->next != NULL)
		{
			if (prev == NULL) // the node we're removing is table[i], WITH a next node
			{
				m->table[i] = curr->next;
				free(curr->key);
				free(curr);

			}
			else if (prev != NULL) // therefore there should ALSO be a prev node
			{
				prev->next = curr->next;
				free(curr->key);
				free(curr);
			}
		}

		m->size--;

		if (m->size < m->num_chains / 4)
		{
			// in this case, our table is too sparse and we can shrink it
			gmap_embiggen(m, m->num_chains / 2);
		}
		
		return return_value;
	}
	else if (n == NULL)
	{
		return NULL;
	}
}

void gmap_for_each(gmap *m, void (*f)(const void *, void *, void *), void *arg)
{
  if (m == NULL || f == NULL)
    {
      return;
    }

  for (size_t chain = 0; chain < m->num_chains; chain++)
    {
      gmap_node *curr = m->table[chain];
      while (curr != NULL)
	  {
	    f(curr->key, curr->value, arg);
	    curr = curr->next;
	  }
    }
}


/**
 * A location in an array where a key can be stored.  The location is
 * represented by a (array, index) pair.
 */
typedef struct _gmap_store_location
{
  const void **arr;
  size_t index;
} gmap_store_location;



/**
 * Stores the given key in the given location and increments the location
 * by one element.
 *
 * @param key a pointer to a key, non-NULL
 * @param value ignored
 * @param arg a pointer to a gmap_store_location
 */
void gmap_store_key_in_array(const void *key, void *value, void *arg)
{
  gmap_store_location *where = arg;
  where->arr[where->index] = key;
  where->index++;
}

const void **gmap_keys(gmap *m)
{
	const void **keys = malloc(sizeof(*keys) * m->size);

	if (keys != NULL)
	  {
	    gmap_store_location loc = {keys, 0};
	    gmap_for_each(m, gmap_store_key_in_array, &loc);
	  }

	return keys;
}	


void gmap_embiggen(gmap *m, size_t n)
{
	if (m == NULL) return;
	if (n <= 0) // check for this case outside of this function in case it makes m invalid
	{
		return;
	}
	// initialize the table
	gmap_node **new_table = calloc(sizeof(gmap_node *), n);

	// grabs every old node in the old table
	for (size_t chain = 0; chain < m->num_chains; chain++)
    {
      gmap_node *curr = m->table[chain];

      while (curr != NULL)
	  {
	  	size_t i = gmap_compute_index(curr->key, m->hash, n);

	  	gmap_node *storednext = curr->next;

	  	// if you get a collision while rehashing
	  	if (new_table[i] != NULL)
	  	{
	  		// don't confuse yourself:
	  		// you can CHANGE your next pointers
	  		// and they are independent of what your
	  		// old next pointers are
	  		// same with prev

	  		// ex: storednext points TO the next node,
	  		// so changing curr->next only changes
	  		// what curr's next is pointing to

	  		curr->next = new_table[i];
	  		new_table[i] = curr;

	  	}
	  	// new_table[i] is null, just put it in
	  	else if (new_table[i] == NULL)
	  	{
	  		// make sure you don't carry over the next info
	  		// to the new table

	  		curr->next = NULL;
	  		new_table[i] = curr;
	  	}

	    curr = storednext;
	  }
    }

	m->num_chains = n;
	free(m->table); // you have to free the old floating table that you malloc'd
	m->table = new_table;
}

