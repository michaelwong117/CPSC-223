#include <stdlib.h>
#include <stdio.h>

#include "minheap.h"

#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x) / 2)

void pqueue_heapify(pqueue *q, size_t idx);


pqueue *pqueue_create(int (*cmp)(const void *d1, const void *d2), size_t capacity) {
    pqueue *res = NULL;
    res = malloc(sizeof(*res));
    res->cmp = cmp;

    res->data = malloc(capacity * sizeof(*(res->data)));
    res->size = 0;
    res->capacity = capacity;
    return (res);
}

void pqueue_delete(pqueue *q) 
{
    if (NULL == q) return;
    free(q->data);
    free(q);
}
size_t pqueue_size(const pqueue *q)
{
  return q != NULL ? q->size : 0;
}

void pqueue_destroy(pqueue *q, void (*f)(void *))
{
  if (q != NULL)
    {
      // free the remaining items using the given function
    if (f != NULL)
	{
	  for (size_t i = 0; i < q->size; i++)
	    {
	      f(q->data[i]);
	    }
	}

      // free the array
      free(q->data);
      // free the meta-data
      free(q);
    }
}

void pqueue_enqueue(pqueue *q, const void *data) 
{
    size_t i;
    void *tmp = NULL;
    if (q->size >= q->capacity) return;

    
    /* Adds element last */
    q->data[q->size] = (void*) data;
    i = q->size;
    q->size++;
    /* The new element is swapped with its parent as long as its
    precedence is higher */
    while(i > 0 && q->cmp(q->data[i], q->data[PARENT(i)]) > 0) {
        tmp = q->data[i];
        q->data[i] = q->data[PARENT(i)];
        q->data[PARENT(i)] = tmp;
        i = PARENT(i);
    }
}

/**
* Returns the element with the biggest priority from the queue .
*/
void *pqueue_dequeue(pqueue *q) 
{
    if (q == NULL || q->size == 0)
    {
      return NULL;
    }

    void *data = NULL;
    if (q->size < 1) {         
         /* Priority Queue is empty */         
         return NULL;     
    }     
    data = q->data[0];
    q->data[0] = q->data[q->size-1];
    q->size--;
    /* Restore heap property */
    pqueue_heapify(q, 0);
    return (data);
}

/**
* Turn an "almost-heap" into a heap .
*/
void pqueue_heapify(pqueue *q, size_t idx) {
    /* left index, right index, largest */
    void *tmp = NULL;
    size_t l_idx, r_idx, lrg_idx;

    l_idx = LEFT(idx);
    r_idx = RIGHT(idx);

    /* Left child exists, compare left child with its parent */
    if (l_idx < q->size && q->cmp(q->data[l_idx], q->data[idx]) > 0) {
        lrg_idx = l_idx;
    } else {
        lrg_idx = idx;
    }

    /* Right child exists, compare right child with the largest element */
    if (r_idx < q->size && q->cmp(q->data[r_idx], q->data[lrg_idx]) > 0) {
        lrg_idx = r_idx;
    }

    /* At this point largest element was determined */
    if (lrg_idx != idx) {
        /* Swap between the index at the largest element */
        tmp = q->data[lrg_idx];
        q->data[lrg_idx] = q->data[idx];
        q->data[idx] = tmp;
        /* Heapify again */
        pqueue_heapify(q, lrg_idx);
    }
}