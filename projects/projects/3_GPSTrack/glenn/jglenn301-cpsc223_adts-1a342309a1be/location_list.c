#include "location_list.h"

#include <stdlib.h>
#include <stdbool.h>

#include "location.h"

typedef struct _loc_node
{
  location *data;
  struct _loc_node *next;
  struct _loc_node *prev;
} location_node;

struct _loc_list
{
  location_node head;
  location_node tail;
  size_t size;
};

void location_list_initialize(location_list *l);
void location_list_add_node_end(location_list *l, location_node *to_add);
void location_list_remove_node(location_list *l, location_node *to_remove);
void location_list_split(location_list *src, location_list *dest1, location_list *dest2);
void location_list_mergesort(location_list *l, int (*comp)(const location *, const location *, const void *), const void *arg);
void location_list_merge(location_list *dest, location_list *src1, location_list *src2, int (*comp)(const location *, const location *, const void *), const void *arg);


location_list *location_list_create()
{
  location_list *result = malloc(sizeof(*result));

  if (result != NULL)
    {
      location_list_initialize(result);
    }

  return result;
}


void location_list_initialize(location_list *l)
{
  l->head.data = NULL;
  l->tail.data = NULL;
  l->head.next = &l->tail;
  l->tail.prev = &l->head;
  l->head.prev = NULL;
  l->tail.next = NULL;
  l->size = 0;
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

  location_node *curr = l->head.next;
  for (size_t j = 0; j < i; j++)
    {
      curr = curr->next;
    }
  
  return curr->data;
}


bool location_list_add(location_list *l, location *loc)
{
  if (l == NULL || loc == NULL)
    {
      return false;
    }

  location_node *added = malloc(sizeof(*added));
  if (added != NULL)
    {
      added->data = loc;
      location_list_add_node_end(l, added);
      return true;
    }
 else
    {
      return false;
    }
}


void location_list_add_node_end(location_list *l, location_node *to_add)
{
  to_add->next = &l->tail;
  to_add->prev = l->tail.prev;
  to_add->next->prev = to_add;
  to_add->prev->next = to_add;
  
  l->size++;
}


void location_list_remove_node(location_list *l, location_node *to_remove)
{
  location_node *before = to_remove->prev;
  location_node *after = to_remove->next;

  to_remove->next = NULL;
  to_remove->prev = NULL;
  before->next = after;
  after->prev = before;

  l->size--;
}


void location_list_split(location_list *src, location_list *dest1, location_list *dest2)
{
  while (src->size > 0)
    {
      location_node *curr = src->head.next;
      location_list_remove_node(src, curr);
      if (dest1->size > dest2->size)
	{
	  location_list_add_node_end(dest2, curr);
	}
      else
	{
	  location_list_add_node_end(dest1, curr);
	}
    }
}


void location_list_merge(location_list *dest, location_list *src1, location_list *src2, int (*comp)(const location *, const location *, const void *), const void *arg)
{
  while (src1->size > 0 || src2->size > 0)
    {
      if (src2->size == 0
	  || (src1->size > 0 && comp(src1->head.next->data,
				     src2->head.next->data,
				     arg) <= 0))
	{
	  // source 1 is smaller, or src2 is empty; add from head of 1to dest
	  location_node *next = src1->head.next;
	  location_list_remove_node(src1, next);
	  location_list_add_node_end(dest, next);
	}
      else
	{
	  // source 2 is smaller, add to destination
	  location_node *next = src2->head.next;
	  location_list_remove_node(src2, next);
	  location_list_add_node_end(dest, next);
	}
    }
}


void location_list_mergesort(location_list *l, int (*comp)(const location *, const location *, const void *), const void *arg)
{
  if (l->size >= 2)
    {
      location_list evens;
      location_list odds;
      location_list_initialize(&evens);
      location_list_initialize(&odds);

      location_list_split(l, &evens, &odds);
      location_list_mergesort(&evens, comp, arg);
      location_list_mergesort(&odds, comp, arg);
      location_list_merge(l, &evens, &odds, comp, arg);
    }
}

void location_list_sort(location_list *l, int (*comp)(const location *, const location *, const void *), const void *arg)
{
  location_list_mergesort(l, comp, arg);
}

double location_list_closed_distance(const location_list *l)
{
  double total_distance = location_distance(l->head.next->data, l->tail.prev->data);
  for (location_node *curr = l->head.next; curr->next != &l->tail; curr = curr->next)
    {
      total_distance += location_distance(curr->data, curr->next->data);
    }
  return total_distance;
}


void location_list_destroy(location_list *l)
{
  location_node *curr = l->head.next;
  while (curr != &l->tail)
    {
      location_node *old_next = curr->next;
      location_destroy(curr->data);
      free(curr);
      curr = old_next;
    }
  
  l->size = 0;
  free(l);
}

void location_list_for_each(const location_list *l, void (*f)(const location *loc))
{
  location_node *curr = l->head.next;
  while (curr != &l->tail)
    {
      f(curr->data);
      curr = curr->next;
    }
}
