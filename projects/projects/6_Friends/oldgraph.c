#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gmap.h"
#include "string_key.h"
#include "gqueue.h"

typedef struct _undigraph
{
  size_t n;
  size_t capacity;
  bool **matrix;
  gmap *index_map;
  char **names;
} undigraph;


#define UNDIGRAPH_INITIAL_CAPACITY (1000)

undigraph *undigraph_create()
{
    undigraph *g = malloc(sizeof(*g));
    if (g == NULL)
    {
        return g;
    }
    g->n = 0;
    g->capacity = UNDIGRAPH_INITIAL_CAPACITY;

    // a map from vertex labels to their indices in the adjacency matrix
    g->index_map = gmap_create(string_copy, string_compare, string_hash, free);

    // make the adjacency matrix and array to remember names
    g->matrix = malloc(sizeof(*g->matrix) * g->capacity);
    g->names = malloc(sizeof(*g->names) * g->capacity);
    if (g->matrix == NULL || g->names == NULL)
      {
	free(g->matrix);
	free(g->names);
	free(g);
	g = NULL;
      }
    else
      {
	size_t row = 0;
	while (row < g->capacity
	       && (g->matrix[row] = calloc(g->capacity, sizeof(*g->matrix[row]))) != NULL)
	  {
	    row++;
	  }
	if (row < g->capacity)
	  {
	    // there was an allocation failure for one of the rows;
	    // free all the previous rows and everything else
	    for (size_t i = 0; i < row; i++)
	      {
		free(g->matrix[row]);
	      }
	    free(g->matrix);
	    free(g);
	    g = NULL;
	  }
      }
	    
    return g;
}


// free dynamically allocated array indices
void undigraph_for_each_gmap_destroy(const void *key, void *value, void *arg)
{
    free(value);
}


void undigraph_destroy(undigraph *g)
{
  if (g != NULL)
    {
      // free the map from vertex labels to indices
      gmap_for_each(g->index_map, undigraph_for_each_gmap_destroy, NULL);
      gmap_destroy(g->index_map);
      
      // free the matrix
      for (size_t r = 0; r < g->capacity; r++)
	{
	  free(g->matrix[r]);
	}
      free(g->matrix);

      // free the names
      for (size_t v = 0; v < g->n; v++)
	{
	  free(g->names[v]);
	}
      free(g->names);
      
      // free the metadata
      free(g);
    }
}


void undigraph_add_vertex(undigraph *g, char *name)
{
  if (g->n < g->capacity)
    {
      size_t *index = malloc(sizeof(*index));
      char *copy = malloc(strlen(name) + 1);
      if (index != NULL && copy != NULL)
	{
	  *index = g->n;
	  if (gmap_put(g->index_map, name, index))
	    {
	      strcpy(copy, name);
	      g->names[g->n] = copy;
	      g->n++;
	    }
	  else
	    {
	      free(copy);
	      free(index);
	    }
	}
      else
	{
	  free(copy);
	  free(index);
	}
    }
}


bool undigraph_has_vertex(undigraph *g, char *name)
{
    return (gmap_contains_key(g->index_map, name));
}


bool undigraph_has_edge(undigraph *g, char *a, char *b)
{
    if (g != NULL && gmap_contains_key(g->index_map, a) && gmap_contains_key(g->index_map, b) && strcmp(a, b) != 0)
    {
      return g->matrix[*(size_t *)gmap_get(g->index_map, a)][*(size_t *)gmap_get(g->index_map, b)];
    }
    return false;
}


bool undigraph_add_edge(undigraph *g, char *a, char *b)
{
    bool success = false;
    if (!undigraph_has_vertex(g, a))
    {
        undigraph_add_vertex(g, a);
    }
    if (!undigraph_has_vertex(g, b))
    {
        undigraph_add_vertex(g, b);
    }
    if (!undigraph_has_edge(g, a, b) && undigraph_has_vertex(g, a) && undigraph_has_vertex(g, b))
    {
      size_t a_index = *(size_t *)gmap_get(g->index_map, a);
      size_t b_index = *(size_t *)gmap_get(g->index_map, b);
      g->matrix[a_index][b_index] = true;
      g->matrix[b_index][a_index] = true;
      success = true;
    }
    return success;
}

// not in the spec, but you may find implementing this function helpful
static char **undigraph_neighbors(undigraph *g, char *name, size_t *count)
{
  *count = 0;
  return NULL;
}

size_t undigraph_bfs_distance(undigraph *g, char *start, char *target)
{
    if (g == NULL || strcmp(start, target) == 0 || !gmap_contains_key(g->index_map, start) || !gmap_contains_key(g->index_map, target))
    {
        return 0;
    }

    // do-nothing stub
    return 0;
}


char **undigraph_common_neighbors(undigraph *g, char *a, char *b, size_t *count, bool *is_valid_query)
{
    if (g == NULL || !gmap_contains_key(g->index_map, a) || !gmap_contains_key(g->index_map, b))
    {
        return NULL;
    }

    // do-nothing stub
    *count = 0;
    return NULL;
}
