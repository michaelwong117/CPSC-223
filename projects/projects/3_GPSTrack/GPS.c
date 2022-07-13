#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "track.h"
#include "trackpoint.h"
#include "location.h"


void print_point(const trackpoint *p, void *info);


// To-do for tomorrow: 
// Edge case handling (see below)
// figure out why test 23 and test 24 don't pass despite apparently same output
// Log File with all proper parts, including collaborators look what that means)
// OH: Questions for tomorrow: sorting out valgrind errors for merge, timed merge, and closest
// get a lesson in gdb while debugging here



// Getting more points on the actual Pset (edge cases to look for, what did you do, how much case handling)
// error checking, etc. before just submitting. also, how did you use late hours?

// Common tips, ula's opinion
// especially for 223, alot of the edge cases are the same. look back at the other psets and see waht
// errors he has. empty inputs, really long names, that's one place to look. he'll probably use them again

// second thing: every single line on the spec is there for a reason
// is theoretically testable: if the function says ownership is given to the caller
// you can test if it's being freed. if it says inputs could be NULL in the funciton
// then you can check if it's NULL. and if it is, return an error

// read very carefully through the spec to handle these cases




int main(int argc, char*argv[])
{
  if (argc < 3)
  {
    fprintf(stderr,"missing files\n");
    exit(-1);
  }

  FILE *in_file  = fopen(argv[2], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"Could not open %s\n", argv[2]);
    exit(-1); // must include stdlib.h 
  } 

  size_t ptcount1 = 0;

  char in = getc(in_file);

  while (in != EOF)
  {
    if (in == '\n')
    {
      ptcount1++;
    }
    in = getc(in_file);
  }

  fflush(in_file);
  fclose(in_file);

  in_file = fopen(argv[2], "r");

  track *track1 = track_create();

  if (track1 == NULL)
  {
    return -1;
  }

  double earliesttime1;
  double earliesttime2;
  double latesttime1;
  double latesttime2;

  for (int i = 0; i < ptcount1; i++)
  {
    double lat, lon, time;
    if(fscanf(in_file, "%lf %lf %lf", &lat, &lon, &time) != 3)
    {
      return -1;
    }
    if (i == 0)
    {
      earliesttime1 = time;
    }
    if (i == ptcount1-1)
    {
      latesttime1 = time;
    }


    // printf("%lf %lf %lf\n", lat, lon, time);

    location *loc = location_create(lat, lon);
    trackpoint *tpt = trackpoint_create(loc, time);
    if (loc == NULL)
    {
      track_destroy(track1);
      return -1;
    }
    if (tpt == NULL)
    {
      location_destroy(loc);
      track_destroy(track1);
      return -1;
    }
    if (!track_add_point(track1, tpt))
    {
      trackpoint_destroy(tpt);
      track_destroy(track1);
      return -1;
    }
    track_add_point(track1, tpt);
  }


  // printf("input1:\n");
  // track_for_each(track1, print_point, NULL);

  fflush(in_file);
  fclose(in_file);

  in_file = fopen(argv[3], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"Could not open %s\n", argv[3]);
    exit(-1); // must include stdlib.h 
  } 

  size_t ptcount2 = 0;

  in = getc(in_file);

  while (in != EOF)
  {
    if (in == '\n')
    {
      ptcount2++;
    }
    in = getc(in_file);
  }

  fflush(in_file);
  fclose(in_file);

  in_file = fopen(argv[3], "r");

  track *track2 = track_create();

  if (track2 == NULL)
  {
    return -1;
  }

  for (int i = 0; i < ptcount2; i++)
  {
    double lat, lon, time;
    if(fscanf(in_file, "%lf %lf %lf", &lat, &lon, &time) != 3)
    {
      return -1;
    }
    if (i == 0)
    {
      earliesttime2 = time;
    }
    if (i == ptcount2-1)
    {
      latesttime2 = time;
    }

    // printf("%lf %lf %lf\n", lat, lon, time);

    location *loc = location_create(lat, lon);
    trackpoint *tpt = trackpoint_create(loc, time);
    if (loc == NULL)
    {
      track_destroy(track2);
      return -1;
    }
    if (tpt == NULL)
    {
      location_destroy(loc);
      track_destroy(track2);
      return -1;
    }
    if (!track_add_point(track2, tpt))
    {
      trackpoint_destroy(tpt);
      track_destroy(track2);
      return -1;
    }
    track_add_point(track2, tpt);
  }

  fflush(in_file);
  fclose(in_file);


  // printf("input2:\n");
  // track_for_each(track2, print_point, NULL);

  // printf("earliesttime1: %lf\n", earliesttime1);
  // printf("earliesttime2: %lf\n", earliesttime2);

  if (strcmp(argv[1], "-combine") == 0)
  {
    if (track_size(track1) == 0)
    {
      track_merge(track2, track1);
      track_for_each(track2, print_point, NULL);
      track_destroy(track2);
    }
    else if (earliesttime2 < earliesttime1)
    {
      track_merge(track2, track1);
      track_for_each(track2, print_point, NULL);
      track_destroy(track2);

    }
    else if (earliesttime2 == earliesttime1)
    {
      if (latesttime1 < latesttime2)
      {
        track_merge(track1, track2);
        track_for_each(track1, print_point, NULL);
        track_destroy(track1);
      }
      else
      {
        track_merge(track2, track1);
        track_for_each(track2, print_point, NULL);
        track_destroy(track2);
      }

    }
    else
    {
      track_merge(track1, track2);
      track_for_each(track1, print_point, NULL);
      track_destroy(track1);
    }
    // track_merge(track1, track2);
    // printf("combined:\n");

  }
  else if (strcmp(argv[1], "-closest") == 0)
  {
    // printf("testing\n");
    if (ptcount1 == 0 || ptcount2 == 0)
    {
      printf("inf\n");
      return 0;
    }
    double res = track_closest_approach(track1, track2);
    if (res == INFINITY || ptcount1 == 0 || ptcount2 == 0)
    {
      printf("inf\n");
      return 0;
    }
    int finalres = round(res);
    printf("%d\n", finalres);
    track_destroy(track1);
    track_destroy(track2);

  }
  else
  {
    return -1;
  }



}

void print_point(const trackpoint *p, void *info)
{
  printf("%.6f %.6f %.6f\n",
   trackpoint_get_latitude(p),
   trackpoint_get_longitude(p),
   trackpoint_get_time(p));
}
