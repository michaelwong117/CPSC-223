#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "smap.h"

typedef struct _smap_node
{
  char *key;
  void *value;
  struct _smap_node *parent;
  struct _smap_node *left;
  struct _smap_node *right;
} smap_node;

/**
 * Information resulting from a search for the node containing a value:
 * the node (or NULL if not found);
 * the parent or would-be parent (or NULL if n is or would be the root);
 * a pointer to the pointer to the node or would-be node;
 * whether it was found (redundant: can determine from whether n is NULL); and
 * the direction parent to n (undefined if n is or would be the root).
 */
typedef struct
{
  smap_node *parent;
  smap_node *n;
  smap_node **incoming;
  bool found;
  int dir;
} smap_search_result;

struct smap
{
  smap_node *root;
  int size;
};

/**
 * Returns a pointer to a new node containing the item.
 *
 * @param item an integer
 * @return a pointer to a new node containing the item
 */
static smap_node *smap_create_node(const char *key, void *value);

/**
 * Deletes the given node in the given BST.
 *
 * @param s a pointer to a set, non-NULL
 * @param node a node in that set's tree
 */
static void smap_delete_node(smap* m, const smap_search_result *result);

/**
 * Finds the node containing the given integer in the given set's tree,
 * or, if the integer is not there, determines where it would be added.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @param return a pointer to the struct in which to record the results
 * of the search
 */
static void smap_find_node(smap *m, const char * key, smap_search_result *result);

/**
 * Deletes all of the nodes in the tree rooted at the given node.
 * There is no effect if given pointer is NULL.
 *
 * @param n a pointer to a node
 */
static void smap_destroy_subtree(smap_node *n);

/**
 * Prints the contents of the subtree rooted at the given node,
 * or "null" if the given pointer is NULL.
 *
 * @param node a pointer to a node
 * @param level the depth of that node
 */
static void smap_print_subtree(const smap_node *node, int level);

/**
 * Processes the contents of the subtree rooted at the given node.
 * All keys (along with their levels in the tree) will be passed to
 * the given function.
 *
 * @param node a pointer to a node
 * @param level the depth of that node
 */
void smap_for_each_subtree(smap_node *n, void (*f)(const char *, void*, void *), void *);

smap *smap_create(int (*hash)(const char *))
{
  smap *m = malloc(sizeof(smap));

  if (m != NULL)
    {
      m->root = NULL;
      m->size = 0;
    }

  return m;
}

int smap_size(const smap *m)
{
  if (m != NULL)
    {
      return m->size;
    }
  else
    {
      return 0;
    }
}

bool smap_contains_key(const smap *m, const char *key)
{
  if (m == NULL)
    {
      return false;
    }
  else
    {
      smap_search_result result;
      smap_find_node((smap *)m, key, &result); // remove const via cast
      
      return result.found;
    }
}

void *smap_get(smap *m, const char *key)
{
  if (m == NULL)
    {
      return false;
    }
  else
    {
      smap_search_result result;
      smap_find_node((smap *)m, key, &result); // remove const via cast
      
      if (result.found)
	{
	  return result.n->value;
	}
      else
	{
	  return NULL;
	}
    }
}

void smap_find_node(smap *m, const char *key, smap_search_result *result)
{
  // start at root
  smap_node *parent = NULL;
  smap_node *curr = m->root;
  smap_node **incoming = &m->root;
  int dir = 0;

  // keep going while not out of places to look and not at what we want
  while (curr != NULL && (dir = strcmp(key, curr->key)) != 0)
    {
      parent = curr;
      if (dir < 0)
	{
	  // key is < value in current node; go left
	  incoming = &curr->left;
	  curr = curr->left;
	}
      else
	{
	  // key is > value in current node; go right
	  incoming = &curr->right;
	  curr = curr->right;
	}
    }

  // save results in struct
  result->parent = parent;
  result->n = curr;
  result->incoming = incoming;
  result->dir = dir;
  result->found = (curr != NULL);
}

bool smap_put(smap *m, const char *key, void *value)
{
  if (m == NULL)
    {
      return false;
    }
  
  // check if key is already in the map
  smap_search_result result;
  smap_find_node(m, key, &result);
  if (result.found)
    {
      // in the map already; replace the value
      result.n->value = value;
      return true;
    }

  // create new node and link parent to it
  smap_node *new_node = smap_create_node(key, value);
  *result.incoming = new_node;

  // link new node back to parent
  if (new_node != NULL)
    {
      new_node->parent = result.parent;
      m->size++;
  
      return true;
    }
  else
    {
      return false;
    }
}

void *smap_remove(smap *m, const char *key)
{
  if (m == NULL || key == NULL)
    {
      return NULL;
    }
  
  // check if key is already in the map
  smap_search_result result;
  smap_find_node(m, key, &result);
  if (result.found)
    {
      void *old_value = result.n->value;
      smap_delete_node(m, &result);
      return old_value;
    }
  else
    {
      return NULL;
    }
}

smap_node *smap_create_node(const char *key, void *value)
{
  smap_node *result = (smap_node *)malloc(sizeof(smap_node));

  if (result != NULL)
    {
      result->right = NULL;
      result->left = NULL;
      result->key = malloc(sizeof(char) * (strlen(key) + 1));
      if (result->key != NULL)
	{
	  strcpy(result->key, key);
	  result->value = value;
	}
      else
	{
	  free(result);
	  return NULL;
	}
    }

  return result;
}

void smap_for_each(smap *m, void (*f)(const char *, void *, void *), void *a)
{
  if (m != NULL && f != NULL)
    {
      smap_for_each_subtree(m->root, f, a);
    }
}

void smap_for_each_subtree(smap_node *n, void (*f)(const char *, void *, void *), void *a)
{
  if (n != NULL)
    {
      // inorder traversal: left subtree, then node, then right subtree
      // (processes data in sorted order!)
      smap_for_each_subtree(n->left, f, a);
      f(n->key, n->value, a);
      smap_for_each_subtree(n->right, f, a);
    }
}

void smap_destroy(smap *m)
{
  smap_destroy_subtree(m->root);
  m->size = 0;
  m->root = NULL;
  free(m);
}

void smap_destroy_subtree(smap_node *n)
{
  if (n != NULL)
    {
      // postorder traversal to delete nodes
      smap_destroy_subtree(n->left);
      smap_destroy_subtree(n->right);
      free(n->key);
      free(n);
    }
}

void smap_print_subtree(const smap_node *node, int level)
{
  // don't need this any more since we have for_each, but maybe useful
  // for debugging?
  for (int i = 0; i < level; i++)
    {
      printf(" ");
    }
  if (node != NULL)
    {
      printf("level %d: %s\n", level, node->key);
      smap_print_subtree(node->left, level + 1);
      smap_print_subtree(node->right, level + 1);
    }
  else
    {
      printf("null\n");
    }
}

static void smap_delete_node(smap* m, const smap_search_result *result)
{
  // someone else can write this
}
