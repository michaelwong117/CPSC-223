#include "track.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "location.h"
#include "trackpoint.h"

typedef struct _track_node
{
  trackpoint *data;
  struct _track_node *next;
  struct _track_node *prev;
} track_node;

struct _track
{
  track_node head;
  track_node tail;
  size_t size;
};

void track_initialize(track *l);
void track_add_single_node_end(track *t, track_node *added);
void track_remove_node(track *t, track_node *to_remove);

void print_point(const trackpoint *p, void *info);


track *track_create()
{
  track *result = malloc(sizeof(*result));

  if (result != NULL)
  {
    track_initialize(result);
  }

  return result;
}


void track_initialize(track *l)
{
  l->head.data = NULL;
  l->tail.data = NULL;
  l->head.next = &l->tail; // why are these passed by reference? ohh they're the only none pointers
  l->tail.prev = &l->head;
  l->head.prev = NULL;
  l->tail.next = NULL;
  l->size = 0;
}

// whenver you malloc before you return you need to free that memory otherwise you'll get a leak
// if you do track_node *curr = l->head.next, you also need to free this
// because you assume that the create function uses malloc
// we know l that we're passing in here must be created with malloc, so
// if we define parts of l as track_nodes

void track_destroy(track *l)
{
  // curr is like an arrow that points to this node, you're not changing l itself
  // unless you dereference it, like curr->data = 5, and that would actually change l
  // if you do curr->data, it's equivalent to *(curr).data so you're actually going into l
  // but if you do curr = l2, l->head.next reminds the same thing
  if (l == NULL) return;
  // printf("calling destroy\n");
  track_node *curr = l->head.next; 
  while (curr != &l->tail)
    {

      // printf("%p\n", curr); // %p is printing out the pointer to curr
      // printf("freeing: \n");
      // print_point(curr->data, NULL);

      track_node *old_next = curr->next;
      trackpoint_destroy(curr->data);
      // if(curr->data != NULL) // maybe don't free things that are NULL already
      free(curr);
      curr = old_next;
    }
  
  l->size = 0;
  free(l);
}


size_t track_size(const track *l)
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



bool track_add_point(track *t, trackpoint *to_add)
{
  if (t == NULL || to_add == NULL)
  {
    return false;
  }

  track_node *added = malloc(sizeof(*added));

  if (added == NULL) return false;

  added->data = to_add;


  if (t->size == 0 || t->head.next == NULL)
  {
    added->next = &t->tail;
    added->prev = &t->head;
    t->tail.prev = added;
    t->head.next = added;
    t->size++;
    // printf("t->size = 0 case: returning true!\n");
  }
  else
  {
    if (trackpoint_compare(to_add, t->tail.prev->data) <= 0) 
    {
      // printf("time is bad: returning false!\n");
      free(added);
      return false;
    }
    else
    {
      track_add_single_node_end(t, added);
      // printf("latest time < new time case: returning true!\n");=
    }
  }
  // printf("added successfully: returning true!\n");
  return true;

}

void track_add_single_node_end(track *t, track_node *added)
{
  added->next = &t->tail; // to_add of next needs to point to the address of tail
  added->prev = t->tail.prev; // here t->tail.prev is a pointer, while t->tail is not
  added->next->prev = added; // tail prev becomes new trackpoint
  added->prev->next = added; // becomes


  t->size++;

}

/**
 * Returns the time between the first trackpoint and the last trackpoint in
 * the given track, in seconds.  Returns zero if the track is empty.
 *
 * @param t a pointer to a track, non-NULL
 * @return the time between the first and last trackpoints on that track
 */
double track_length(const track *t)
{
  trackpoint *begin = t->head.next->data;
  trackpoint *end = t->tail.prev->data;

  if (begin == NULL || end == NULL || t->size == 0)
  {
    return 0;
  }

  return (trackpoint_get_time(end) - trackpoint_get_time(begin));
}


// can't return the actual location, you have return a COPY of the location, see track.h line 65
location *track_get(const track *t, double time)
{
  if (t == NULL)
  {
    return NULL;
  }
  track_node *curr = t->head.next;
  track_node *end = t->tail.prev;

  if (curr->data == NULL || end->data == NULL || t->size == 0) // track is empty
  {
    return NULL;
  }
  // time is before start time and after last time
  if (time < trackpoint_get_time(curr->data) || time > trackpoint_get_time(end->data)) 
  {
    return NULL;
  }

  for (size_t j = 0; j < t->size; j++)
  {
    if (time == trackpoint_get_time(curr->data))
    {
      location *loc = trackpoint_get_location(curr->data);

      return location_create(location_get_latitude(loc), location_get_longitude(loc));
    }
    curr = curr->next;
  }
  // did not find any matching time, try linear interpolation

  // find the first trackpoints before and after the given time.

  curr = t->head.next;
  trackpoint *after;

  for (size_t j = 0; j < t->size; j++)
  {
    // if these trackpoints times came after our given time
    if (trackpoint_get_time(curr->data) > time)
    { 
      // store the most recent one afterour given time
      after = curr->data;
      break;
    }
    curr = curr->next;
  }

  // going in opposite order through the linked list
  curr = t->tail.prev;
  trackpoint *before;

  for (size_t j = t->size-1; j >= 0; j--)
  {
    // if these trackpoints times came earlier than our given time
    if (trackpoint_get_time(curr->data) < time)
    { 
      // store the most recent one before our given time
      before = curr->data;
      break;
    }
    curr = curr->prev;
  }
  // get the linear interpolation of before and after

  track_segment *seg = track_segment_create(before, after); 

  location *interpolate = track_segment_get_location(seg, time);

  free(seg);

  return interpolate;  
  // return location_create(location_get_latitude(interpolate), location_get_longitude(interpolate));
}

void track_for_each(const track *t, void (*f)(const trackpoint *, void *), void *arg)
{
  track_node *curr = t->head.next;
  while (curr != &t->tail)
  {
    f(curr->data, arg);
    curr = curr->next;
  }
}

void track_remove_node(track *l, track_node *to_remove)
{
  // printf("removing node: \n");
  // print_point(to_remove->data, NULL);
  if (to_remove == NULL) return;

  track_node *before = to_remove->prev;
  track_node *after = to_remove->next;

  to_remove->next = NULL;
  to_remove->prev = NULL;
  before->next = after;
  after->prev = before;

  trackpoint_destroy(to_remove->data);
  free(to_remove);

  l->size--;

}

void k_shared(track *dest, track *src, track_node **start)
{
  *start = dest->tail.prev;
  while (trackpoint_compare((*start)->data, src->head.next->data) != -1)
  {
    *start = (*start)->prev;
  }
}

// pass dest and src by reference

void track_merge(track *dest, track *src)
{
  track_node *earliestdest = dest->head.next;
  track_node *earliestsrc = src->head.next;

  if (dest == NULL || src == NULL || src->size == 0) return;

  if (dest->size == 0 && src->size == 0)
  {
    return;
  }
  else if (dest->size > 0 && src->size == 0)
  {
    return;
  }


  // dest and src are potentially now switched
  earliestdest = dest->head.next;
  earliestsrc = src->head.next;
  track_node *latestdest = dest->tail.prev;

  // printf("last point in dest:  ");
  // print_point(latestdest->data, NULL);
  // printf("first point in src: ");
  // print_point(earliestsrc->data, NULL);
  // if the last value in destination is still earlier than the earliest value in the source

  if (trackpoint_compare(latestdest->data, earliestsrc->data) == -1)
  {
    // the last one in destination's next to be the first in source
    dest->tail.prev->next = src->head.next;

    // the first one in source's previous should no longer point to src head
    // it should point to the last one in destination
    src->head.next->prev = dest->tail.prev;

    // the last one in destination should be the last one in source
    // e.g., source is the ending of dest before the tail
    dest->tail.prev = src->tail.prev;

    // the last one in source's next should point to the destination tail
    src->tail.prev->next= &dest->tail; // remember, the tail and head are not pointers, everything else is!


    // manging source, should be an empty linked list with its head and tail pointing to one another
    src->tail.prev = &src->head;
    src->head.next = &src->tail;

    //also change the sizes
    dest->size += src->size;
    
    track_destroy(src);

    return;
  }

  // this marks the start of the overlap

  track_node *srccurr = src->head.next;

  // printf("starting point in src:  ");
  // print_point(srccurr->data, NULL);


  // destcurr was hitting desttail
  track_node *enddest = &dest->tail;
  track_node *endsrc = &src->tail;

  track_node* start = NULL;
  
  // there is some value in dest that is greater than a value in src
  track_node *destcurr = dest->head.next;

  if (trackpoint_compare(dest->head.next->data, src->head.next->data) == -1 && trackpoint_compare(dest->tail.prev->data, src->tail.prev->data) == -1)
  {
    k_shared(dest, src, &start);
    destcurr = start;
  }
  else if (trackpoint_compare(dest->head.next->data, src->head.next->data) == -1 && trackpoint_compare(dest->tail.prev->data, src->tail.prev->data) == 1)
  {
    k_shared(dest, src, &start);
    destcurr = start;
  }

  while (srccurr != &src->tail)
  {

    if (trackpoint_compare(latestdest->data, srccurr->data) == -1)
    {

      // printf("APPEND CASE: srccurr is currently greater than last dest time!\n\n");
      // printf("latestdest:  ");
      // print_point(latestdest->data, NULL); // next is null?
      // printf("src curr:  ");
      // print_point(srccurr->data, NULL);

      // the last one in destination's next to be the first in source
      dest->tail.prev->next = srccurr;

      // printf("after src curr:  ");
      // print_point(dest->tail.prev->next->data, NULL);

      // the first one in source's previous should no longer point to src head
      // it should point to the last one in destination

      // printf("srccurr->prev: \n");
      // print_point(srccurr->prev->data, NULL);
      srccurr->prev = dest->tail.prev;

      // printf("srccurr->prev: \n");
      // print_point(srccurr->prev->data, NULL);

      // the last one in destination should be the last one in source
      // e.g., source is the ending of dest before the tail
      dest->tail.prev = src->tail.prev;

      // the last one in source's next should point to the destination tail
      src->tail.prev->next= &dest->tail; // remember, the tail and head are not pointers, everything else is!


      // manging source, should be an empty linked list with its head and tail pointing to one another
      src->tail.prev = &src->head;
      src->head.next = &src->tail;


      //also change the sizes
      dest->size += src->size; // triple check this

      track_destroy(src);


      return;
    }
    // if (trackpoint_compare(destcurr->data, src->tail.prev->data) == 1)
    // {
    //   print_point(destcurr->data, NULL);
    //   print_point(src->tail.prev->data, NULL);
    //   // handle the the tail and nodes before srccurr

    //   src->tail.prev->next = destcurr; // need to change these first before changing tail.prev
    //   destcurr->prev = src->tail.prev; 

    //   src->tail.prev = srccurr->prev;
    //   srccurr->prev->next = &src->tail;


    //   destcurr->prev->next = srccurr;
    //   srccurr->prev = destcurr->prev;

    //   // handle the the tail and nodes before srccurr

    //   // src->tail.prev = &src->head;
    //   // src->head.next = &src->tail;


    //   // src->tail.prev->next = destcurr;

    //   dest->size += src->size;
      
    //   track_destroy(src);

    //   return;
    // }

    if (destcurr == &dest->tail)
    {
      printf("ERROR: shouldn't reach here");
      return;
    }
    // in the normal case
    // this is where the valgrind error is
    if (trackpoint_compare(destcurr->data, srccurr->data) == 0) // the times are equal
    {
      // if the locations are identical, continue
      if (location_compare(trackpoint_get_location(destcurr->data), trackpoint_get_location(srccurr->data)) == 0)
      {
        // printf("IDENTICAL CASE: src and dest are equal!\n\n");
        // printf("dest curr:  ");
        // print_point(destcurr->data, NULL);
        // printf("src curr:  ");
        // print_point(srccurr->data, NULL);

        // srccurr->next->prev = srccurr->prev;
        srccurr = srccurr->next;
        destcurr = destcurr->next; // THE STRUCTURES OF THESE NODES ARE WRONG: THEY DON'T REACH TAIL!

        if (srccurr->prev != &src->head)
        {
          track_remove_node(src, srccurr->prev);
        }
        // 315 630
      }

      // otherwise, remove the node
      else
      {
        // Take a closer look at this case

        // printf("IDENTICAL TIMES, DIFF LOCATIONS: \n\n");
        // printf("dest curr:  ");
        // print_point(destcurr->data, NULL);
        // printf("src curr:  ");
        // print_point(srccurr->data, NULL);

        destcurr = destcurr->next;
        srccurr = srccurr->next;

        track_remove_node(dest, destcurr->prev); // size gets decreased in remove_node

        if (srccurr->prev != &src->head)
        {
          track_remove_node(src, srccurr->prev);
        }

      }

    }
    else if (trackpoint_compare(destcurr->data, srccurr->data) == -1) // dest curr is earlier than src curr
    {
      // the order is fine as is: 61 in dest < 72 in src, keep incrementing in dest until you find
                                                  // a value greater than 72, then you need to add before that
      // printf("DESTCURR IS LESS:\n\n");
      // printf("dest curr:  ");
      // print_point(destcurr->data, NULL);
      // printf("src curr:  ");
      // print_point(srccurr->data, NULL);

      destcurr = destcurr->next;
    }
    // we found a value in src that is less than the current dest
    // e.g.: destcurr = 75.0 vs srccurr = 71.0
    else
    {
      // we need to put this earlier time of source before the current destination value
      // so we link it to the previous current node

      // printf("DESTCURR IS GREATER:\n\n");
      // printf("dest curr:  ");
      // print_point(destcurr->data, NULL);
      // printf("src curr:  ");
      // print_point(srccurr->data, NULL);

      //dest: 52 58 75
      //src: 52 66 71 80

      // 75 would be our destcurr
      // 66 would be our srccurr
      
      // When you dereference a pointer, you can change the value, and if any other pointer
      // was also pointing to that memory address will also have its dereferenced value changed.


      // before we do any swaps, we store the pointer to next. even if we change the value of srccurr->next,
      track_node *nextinsrc = srccurr->next;  

      // handle the nodes around srccurr, see diagram b in notebook

      srccurr->prev->next = srccurr->next;
      srccurr->next->prev = srccurr->prev;


      destcurr->prev->next = srccurr; // destcurr's previous should have a next pointer to srccurr

      srccurr->prev = destcurr->prev; // srccurr should have a previous pointer to destcurr's previous

      destcurr->prev = srccurr; // destcurr should have a previous pointer to srccurr


      // page now looks like diagram A in notebook

      srccurr->next = destcurr;

      //handling the nodes around srccurr; do i need to handle this?



      srccurr = nextinsrc; // increment srccurr node

      src->size--;
      dest->size++;

    }

  }

  track_destroy(src);
}



double track_closest_approach(const track *track1, const track *track2)
{
  track_node *curr1 = track1->head.next;
  track_node *curr2 = track2->head.next;

  double min = INFINITY;
  double min_time = -1;

  if (curr1 == NULL || curr2 == NULL)
  {
    return INFINITY; 
  }


  track_node *t1end = track1->tail.prev;
  track_node *t2end = track2->tail.prev;
  
  while (trackpoint_compare(curr1->data, track2->head.next->data) == -1) 
  {
    curr1 = curr1->next; 
  }
  
  while (trackpoint_compare(curr2->data, track1->head.next->data) == -1) 
  { 
    curr2 = curr2->next; 
  }

  while (trackpoint_compare(t1end->data, track2->tail.prev->data) == 1) 
  { 
    t1end = t1end->prev; 
  }

  while (trackpoint_compare(t2end->data, track1->tail.prev->data) == 1) 
  { 
    t2end = t2end->prev; 
  }



  while(curr1 != t1end && curr2 != t2end)
  {
    // printf("\nCHECKING CURR VALUES: \n");
    // printf("curr1:\n");
    // print_point(curr1->data);
    // printf("curr2:\n");
    // print_point(curr2->data);
    // printf("\n");

    if (trackpoint_compare(curr1->data, curr2->data) == 0) // the times are equal
    {
      double dist = location_distance(trackpoint_get_location(curr1->data), 
                                      trackpoint_get_location(curr2->data));

      if (dist < min)
      {
        min = dist;
        min_time = trackpoint_get_time(curr1->data);

      }

      // printf("EQUAL CASE:\n");

      // printf("curr1:\n");
      // print_point(curr1->data);
      // printf("curr2:\n");
      // print_point(curr2->data);

      // printf("min dist %lf at time %lf\n", min, min_time);

      curr1 = curr1->next;
      curr2 = curr2->next;

    } 
    // if curr1's time is less than curr2's time
    // use the location of curr2->prev (the node previous to curr2)
    // as the starting point to the ending point of curr2's location
    // and the time between them in the location_interpolate equation
    // to find the time for location_interpolate

    else if (trackpoint_compare(curr1->data, curr2->data) == -1)
    {
      if (curr2 == track2->head.next)
      {
        curr2 = curr2->next;
        continue;
      }

      // printf("Curr1 LESSER CASE:\n");

      // printf("curr1:\n");
      // print_point(curr1->data);
      // printf("curr2:\n");
      // print_point(curr2->data);

      // printf("curr2->prev:\n");
      // print_point(curr2->prev->data);

      location *prev = trackpoint_get_location(curr2->prev->data);
      location *intp = trackpoint_get_location(curr2->data);

      double t1 = trackpoint_get_time(curr2->prev->data);
      double t2 = trackpoint_get_time(curr1->data); // this is the time in the middle
      double t3 = trackpoint_get_time(curr2->data);


      // printf("%lf %lf %lf\n", t1, t2, t3);

      location *loc1 = trackpoint_get_location(curr1->data);

      double w = (t2 - t1) / (t3 - t1);

      location *check = location_interpolate(prev, intp, w);

      double dist = location_distance(check, loc1);

      if (dist < min)
      {
        min = dist;
        min_time = t2; // don't set min_time = w; that's your w proportion for interpolate
      }

      // printf("min dist %lf at time %lf\n", min, min_time);

      // location_destroy(prev);
      // location_destroy(intp);
      // location_destroy(loc1);
      location_destroy(check);

      curr1 = curr1->next;
    }
    // if curr2's time is less than curr1's time (yellow, t=36 less than red t=45)
    else
    {
      if (curr1 == track1->head.next)
      {
        curr1 = curr1->next;
        continue;
      }
      // printf("Curr1 GREATER CASE:\n");

      // printf("curr1:\n");
      // print_point(curr1->data);
      // printf("curr2:\n");
      // print_point(curr2->data);


      // printf("curr1->prev:\n");
      // print_point(curr1->prev->data);

      location *prev = trackpoint_get_location(curr1->prev->data);
      location *intp = trackpoint_get_location(curr1->data);


      double t1 = trackpoint_get_time(curr1->prev->data);
      double t2 = trackpoint_get_time(curr2->data); // this is the time in the middle
      double t3 = trackpoint_get_time(curr1->data);

      // printf("%lf %lf %lf\n", t1, t2, t3);

      location *loc2 = trackpoint_get_location(curr2->data);

      double w = (t2 - t1) / (t3 - t1);

      location *check = location_interpolate(prev, intp, w);

      double dist = location_distance(check, loc2);

      if (dist < min)
      {
        min = dist;
        min_time = t2; // don't set min_time = w; that's your w proportion for interpolate
      }
      // printf("min dist %lf at time %lf\n", min, min_time);

      // location_destroy(prev);
      // location_destroy(intp);
      // location_destroy(loc2);
      location_destroy(check);

      curr2 = curr2->next;
    }

  }
  return min;
}



