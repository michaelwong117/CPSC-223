#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "gmap.h"
#include "string_key.h"
#include "gqueue.h"

typedef struct _undigraph
{
  size_t n;
  size_t capacity;
  // bool **matrix; // remove this because add_vertex will have to grow adj matrix
  gmap **adjset;
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
    // with newly included adjacency list, maps vertext labels to their indices
    // in the adjacency list
    g->index_map = gmap_create(string_copy, string_compare, string_hash, free);

    // make the adjacency list and array to remember names
    g->names = malloc(sizeof(*g->names) * g->capacity);
    g->adjset = malloc(sizeof(*g->adjset) * g->capacity);

  if (g->names == NULL || g->adjset == NULL)
    {
      free(g->names);
      free(g->adjset);
      free(g);
      g = NULL;
    }
  else
  {
	  size_t row = 0;
	  while (row < g->capacity
         && (g->adjset[row] = gmap_create(string_copy, string_compare, string_hash, free)) != NULL)
	  {
	    row++;
	  }
	  if (row < g->capacity)
	  {
	    // there was an allocation failure for one of the rows;
	    // free all the previous rows and everything else
	    for (size_t i = 0; i < row; i++)
	    {
         gmap_destroy(g->adjset[row]);
	    }
      free(g->adjset);
      free(g);
      g = NULL;
	  }
  }
	    
  return g;
}
// check with ulas
void undigraph_embiggen(undigraph *g, size_t n)
{
  g->capacity = n;

  // printf("we good for now?\n");

  // realloc the adjacency list and array to remember names
  g->names = (char **) realloc(g->names, sizeof(*g->names) * g->capacity);
  g->adjset = (gmap **) realloc(g->adjset, sizeof(*g->adjset) * g->capacity);

  if (g->names == NULL || g->adjset == NULL)
  {
    free(g->names);
    free(g->adjset);
    free(g);
    g = NULL;
    return;
  }

  size_t row = n / 2;
  while (row < g->capacity
        && (g->adjset[row] = gmap_create(string_copy, string_compare, string_hash, free)) != NULL)
  {
    row++;
  }
  if (row < g->capacity)
  {
    // there was an allocation failure for one of the rows;
    // free all the previous rows and everything else
    for (size_t i = 0; i < row; i++)
    {
        gmap_destroy(g->adjset[row]);
    }
    free(g->adjset);
    free(g);
    g = NULL;
  }
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
    
    // free the adj set
    for (size_t r = 0; r < g->capacity; r++)
	  {
      gmap_destroy(g->adjset[r]);
	  }
    free(g->adjset);

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
  if (g->n >= g->capacity)
  {
    // printf("embiggening\n");
    undigraph_embiggen(g, g->capacity * 2);
    // printf("g->capacity: %ld\n", g->capacity);
  }

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


bool undigraph_has_vertex(undigraph *g, char *name)
{
    return (gmap_contains_key(g->index_map, name));
}


bool undigraph_has_edge(undigraph *g, char *a, char *b)
{
    if (g != NULL && gmap_contains_key(g->index_map, a) && gmap_contains_key(g->index_map, b) && strcmp(a, b) != 0)
    {
      // adjset[the index of string a in index map ] -> the set of edges for vertex a
      // contains_key(adjset[index a], b) finds whether b is a key that exists in set a
      return gmap_contains_key(g->adjset[*(size_t *)gmap_get(g->index_map, a)], b);
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

      // adjset[the index of string a in index map ] -> the set of edges for vertex a
      // contains_key(adjset[index a], b) finds whether b is a key that exists in set a
    
      // adds key of b with value of a in the vertex a's map; represents edge [a,b]
      // for has_edge gmap_contains_key(adjset[gmap_get(g->index_map, a)], b);

      gmap_put(g->adjset[a_index], b, a);
      // adds key of a with value of b in vertex b's map
      gmap_put(g->adjset[b_index], a, b);
      
      success = true;

    }
    
    return success;
}

size_t undigraph_bfs_distance(undigraph *g, char *start, char *target)
{
    if (g == NULL || strcmp(start, target) == 0 || !gmap_contains_key(g->index_map, start) || !gmap_contains_key(g->index_map, target))
    {
        return 0;
    }

    int *d = malloc(sizeof(int) * g->n);
    int *color = malloc(sizeof(int) * g->n);
    int *pred = malloc(sizeof(int) * g->n);

    if (d == NULL || color == NULL || pred == NULL)
    {
      free(d);
      free(color);
      free(pred);
      return 0;
    }

    for (int i = 0; i < g->n; i++)
    {
      d[i] = 0; // there does not exist a path from start to node i
      color[i] = 0; // unseen color
      pred[i] = -1; // null predescessor
    }

    gqueue *q = gqueue_create();

    size_t *start_idx = malloc(sizeof(size_t));

    if (start_idx == NULL) // freeing if malloc fails
    {
      free(d);
      free(color);
      free(pred);
      gqueue_destroy(q);
      return 0;
    }

    *start_idx = *(size_t *) gmap_get(g->index_map, start);
    
    color[*start_idx] = 1; // in processing

    gqueue_enqueue(q, start_idx); // enqueue the index of start

    size_t targetidx = *(size_t *) gmap_get(g->index_map, target);

    // if queue is not empty and targetidx has not been seen, continue searching
    while(!is_gqueue_empty(q) && d[targetidx] == 0)
    {
      // call this vertex u
      size_t *u_idx = (size_t *) gqueue_dequeue(q);
      gmap *adju = g->adjset[*u_idx];

      // printf("dequeuing node %s in the queue\n", g->names[*u_idx]);

      if (adju != NULL)
      {
        char **neighbors = (char **) gmap_keys(adju);

        // get all neighbors v of vertex u
        for (int i = 0; i < gmap_size(adju); i++)
        {
          size_t *v_idx = malloc(sizeof(size_t));
          if (v_idx == NULL)
          {
            free(d);
            free(color);
            free(pred);
            gqueue_destroy(q);
            free(neighbors);
            return 0;
          }
          *v_idx = *(size_t *) gmap_get(g->index_map, neighbors[i]);

          if (color[*v_idx] == 0)
          {
            gqueue_enqueue(q, v_idx);
            d[*v_idx] = d[*u_idx] + 1;
            color[*v_idx] = 1;
            pred[*v_idx] = *u_idx;
          }
          else
          {
            free(v_idx);
          }
        }
        free(neighbors);
      }
      color[*u_idx] = 2;
      free(u_idx);
    }

    gqueue_destroy(q);

    int res = d[*(size_t *) gmap_get(g->index_map, target)];

    free(d);
    free(color);
    free(pred);

    return res;
}

/**
 * Returns all common neighbors to two vertices in the supplied graph
 *
 * @param g a pointer to an undigraph, non NULL
 * @param a the first vertex of the query
 * @param b the second vertex of the query
 * @param count the number of neighbors common to both a and b, initialized to 0 by the caller
 * @param successful_query the truthiness of whether or not the query succeeded
 * without any memory issues, initialized to false by the caller
 * @return an array of pointers to unsorted names, each of which having been copied.
 * Ownership of these copied names and the containing array is transferred to the caller.
 * 
 * Both a and b are guaranteed to be present in g. NULL is returned in two cases:
 * 1) there was an allocation error or 2) there are no common neighbors.
 * If NULL is returned and successful_query is false, a memory issue occurred.
 * If NULL is returned otherwise, no memory issue occurred.
 * 
 * Expected runtime: O(D' * S' + D'' * S''), although it is a precondition that no string
 * may have length greater than 20, collapsing the bound to O(D' + D'')
 */

char **undigraph_common_neighbors(undigraph *g, char *a, char *b, size_t *count, bool *is_valid_query)
{
    if (g == NULL || !gmap_contains_key(g->index_map, a) || !gmap_contains_key(g->index_map, b))
    {
      *is_valid_query = true; // no memory issues, still return NULL
      return NULL;
    }
    
    *count = 0;
    size_t a_idx = *(size_t *) gmap_get(g->index_map, a);
    size_t b_idx = *(size_t *) gmap_get(g->index_map, b);

    // printf("a and b in paremeters: %s %s\n", a, b);
    // printf("a and b indexes: %d %d\n", a_idx, b_idx);
    // printf("a and b in names: %s %s\n", g->names[a_idx], g->names[b_idx]);

    int *neighborcnt = calloc(g->n, sizeof(int));

    if (neighborcnt == NULL) // freeing if malloc fails
    {
      free(neighborcnt);
      return NULL; // is_valid_query is false
    }

    gmap *adj_a = g->adjset[a_idx];
    gmap *adj_b = g->adjset[b_idx];

    char **neighborsa = (char **) gmap_keys(adj_a);
    char **neighborsb = (char **) gmap_keys(adj_b);

    if (neighborsa == NULL || neighborsb == NULL)
    {
      free(neighborcnt);
      free(neighborsa);
      free(neighborsb);
      return NULL;
    }

    for (int i = 0; i < gmap_size(adj_a); i++)
    {
      size_t v_idx = *(size_t *) gmap_get(g->index_map, neighborsa[i]);
      neighborcnt[v_idx]++;
    }
    for (int i = 0; i < gmap_size(adj_b); i++)
    {
      size_t v_idx = *(size_t *) gmap_get(g->index_map, neighborsb[i]);
      neighborcnt[v_idx]++;
    }
    for (int i = 0; i < g->n; i++)
    {
      if (neighborcnt[i] == 2) 
      {
        (*count)++; // increment the counter, neighbor of both a and b
      }
    }

    char **commonneighbors = malloc(sizeof(char *) * (*count));
    if (commonneighbors == NULL)
    {
      free(neighborcnt);
      free(neighborsa);
      free(neighborsb);
      return NULL;
    }
    int cnt = 0;
    for (int i = 0; i < g->n; i++)
    {
      if (neighborcnt[i] == 2)
      {
        commonneighbors[cnt] = string_copy(g->names[i]);
        cnt++;
        // printf("%s\n", g->names[i]);
      }
    }

    free(neighborcnt);
    free(neighborsa);
    free(neighborsb);

    *is_valid_query = true;

    if (*count == 0) // there are no common neighbors
    {
      free(commonneighbors);
      return NULL;
    }
    return commonneighbors;
}

// every case that's listed in the spec, test it
// bfs_distance (specifically says what's supposed to happen when there is no path)
// not testing when the preconditions of the functions aren't met
// if the spec says two vertices in the function are distinct, they will be distinct (for has_edge)
// friends have to test the invalid command arguments
// test all the different ways the end user could give you invalid input
