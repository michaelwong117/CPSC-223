#include "smap.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *smap_error = "error";

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Blotto competition program.
 */

typedef struct _smap_node
{
  char *key;
  void* value;
  struct _smap_node *parent;
  struct _smap_node *left;
  struct _smap_node *right;
} smap_node;

struct _smap
{
  smap_node *root;
  size_t size;
};

smap_node *smap_table_find_key(smap *m, const char *key, smap_node **parent, smap_node ***incoming);
const smap_node *smap_table_find_key_const(const smap *m, const char *key);

void smap_for_each_helper(smap_node *curr, void (*f)(const char *, void *, void *), void *arg);

void smap_destroy_helper(smap_node *curr);


smap *smap_create(size_t (*h)(const char *s))
{
  // this implementation doesn't need the hash function so we skip the
  // check that it is not NULL
  
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->root = NULL;
      result->size = 0;
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

/**
 * Finds the node in the given map containing the given key, or finds
 * where it would go if it is not present.
 * 
 */
const smap_node *smap_table_find_key_const(const smap *m, const char *key)
{
  const smap_node *curr = m->root;

  int dir;
  while (curr != NULL && (dir = strcmp(key, curr->key)) != 0)
    {
      if (dir < 0)
	{
	  curr = curr->left;
	}
      else
	{
	  curr = curr->right;
	}
    }
  return curr;
}

smap_node *smap_table_find_key(smap *m, const char *key, smap_node **parent, smap_node ***incoming)
{
  smap_node *curr = m->root;
  *incoming = &m->root;
  *parent = NULL;

  int dir;
  while (curr != NULL && (dir = strcmp(key, curr->key)) != 0)
    {
      *parent = curr;
      if (dir < 0)
	{
	  *incoming = &curr->left;
	  curr = curr->left;
	}
      else
	{
	  *incoming = &curr->right;
	  curr = curr->right;
	}
    }
  return curr;
}

void *smap_put(smap *m, const char *key, void *value)
{
  if (m != NULL && key != NULL)
    {
      // find key
      smap_node *parent;
      smap_node **incoming;
      smap_node *node = smap_table_find_key(m, key, &parent, &incoming);
      if (node != NULL)
	{
	  // found key; replace value
	  void *old_value = node->value;
	  node->value = value;
	  return old_value;
	}
      else
	{
	  // new key; make a new node
	  smap_node *new_node = malloc(sizeof(*new_node));
	  
	  if (new_node == NULL)
	    {
	      return smap_error;
	    }
	  
	  // make a copy of the new key
	  char *copy = malloc(strlen(key) + 1);
	  strcpy(copy, key);
	  
	  if (copy != NULL)
	    {
	      new_node->key = copy;
	      new_node->value = value;
	      new_node->parent = parent;
	      new_node->left = NULL;
	      new_node->right = NULL;
	      *incoming = new_node;
	      m->size++;
	      return NULL;
	    }
	  else
	    {
	      free(new_node);
	      return smap_error;
	    }
	}
    }
  else
    {
      // NULL matrix or key
      return smap_error;
    }
}

void *smap_remove(smap *m, const char *key)
{
  // TO-DO: write and test remove
  return NULL;
}

bool smap_contains_key(const smap *m, const char *key)
{
  return smap_table_find_key_const(m, key) != NULL;
}

void *smap_get(smap *m, const char *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }

  const smap_node *node = smap_table_find_key_const(m, key);
  if (node != NULL)
    {
      return node->value;
    }
  else
    {
      return NULL;
    }
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

  smap_for_each_helper(m->root, f, arg);
}  


void smap_for_each_helper(smap_node *curr, void (*f)(const char *, void *, void *), void *arg)
{
  if (curr != NULL)
    {
      smap_for_each_helper(curr->left, f, arg);
      f(curr->key, curr->value, arg);
      smap_for_each_helper(curr->right, f, arg);
    }
}

void smap_destroy(smap *m)
{
  if (m == NULL)
    {
      return;
    }

  smap_destroy_helper(m->root);

  // free the smap struct
  free(m);
}

void smap_destroy_helper(smap_node *curr)
{
  if (curr != NULL)
    {
      smap_destroy_helper(curr->left);
      smap_destroy_helper(curr->right);
      free(curr->key);
      free(curr);
    }
}
