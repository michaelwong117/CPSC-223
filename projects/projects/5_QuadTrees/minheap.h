#ifndef __MINHEAP_H__
#define __MINHEAP_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct _pqueue_ 
{
    size_t size;
    size_t capacity;
    void **data;
    int (*cmp)(const void *d1, const void *d2);
} pqueue;

pqueue *pqueue_create(int (*cmp)(const void *d1, const void *d2), size_t capacity);

size_t pqueue_size(const pqueue *q);

void pqueue_destroy(pqueue *q, void (*f)(void *));

void pqueue_enqueue(pqueue *q, const void *data);

void *pqueue_dequeue(pqueue *q);

#endif
