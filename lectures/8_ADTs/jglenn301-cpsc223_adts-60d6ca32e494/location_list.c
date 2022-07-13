 #include "location_list.h"

#include <stdlib.h>
#include <stdbool.h>

#include "location.h"

//node struct

typedef struct _loc_node location_node;

typedef struct _loc_node
{ 
  location *data;
  location_node prev;  
  location_node next;
};

struct _loc_list
{
  location_node head;
  location_node tail;
  size_t size;
};

#define LOCATION_LIST_INITIAL_CAPACITY 2


location_list *location_list_create()
{
  location_list *result = malloc(sizeof(*result));
  if (result != NULL)
    {
      l->head.data = NULL;  
      l->tail.data = NULL; 
      l->head.prev = NULL;
      l->tail.next = NULL; 
      l->head.next = &l->tail;
      l->tail.prev = &l->head; 

    }

  return result;
}

size_t location_list_size(const location_list *l)
{
  if (l == NULL)
    {
      return 0;
    }
  else
    {
      return l->size;
    }
}

location *location_list_get(const location_list *l, size_t i)
{
  if (l == NULL || i < 0 || i >= l->size)
    {
      return NULL;
    }
  
  return l->elements[i];
}

bool location_list_add(location_list *l, location *c)
{
  if (l == NULL || c == NULL)
    {
      return false;
    }

  if (l->capacity > 0 && l->size == l->capacity)
    {
      location **bigger = realloc(l->elements, sizeof(*l->elements) * l->capacity * 2);
      if (bigger != NULL)
	{
	  l->elements = bigger;
	  l->capacity *= 2;
	}
    }
  
  if (l->size < l->capacity)
    {
      l->elements[l->size] = c;
      l->size++;
      return true;
    }
  else
    {
      return false;
    }
}

void location_list_sort_lon(location_list *l)
{
  // lazy: selection sort of locations by increasing longitude
  for (size_t i = 0; i < l->size - 1; i++)
    {
      size_t loc_max = i;
      for (size_t j = i + 1; j < l->size; j++)
	{
	  if (location_compare(l->elements[j], l->elements[loc_max]) > 0)
	    {
	      loc_max = j;
	    }
	}
      location *temp = l->elements[i];
      l->elements[i] = l->elements[loc_max];
      l->elements[loc_max] = temp;
    }
}

double location_list_closed_distance(const location_list *l)
{
  double total_distance = 0.0;
  for (size_t i = 0; i < l->size; i++)
    {
      total_distance += location_distance(l->elements[i], l->elements[(i + 1) % l->size]);
    }
  return total_distance;
}


void location_list_destroy(location_list *l)
{
}
