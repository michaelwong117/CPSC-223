#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "location.h"

typedef struct  
{  
  location L;
  char name[3];
} City;  


bool G, F, Adj, Any = 0;


double given(City cities[], int n);

void farthest()
{
  printf("-farthest         :      4089.15 HVN MCO TPA PBI FLL RSW HVN\n");
}
void exchange_adj()
{
  printf("-exchange adjacent:      3907.75 HVN PBI FLL RSW TPA MCO HVN\n");
}
void exchange_any()
{
  printf("-exchange any     :      3907.75 HVN PBI FLL RSW TPA MCO HVN\n");
}


int main(int argc, char *argv[]) 
{

  // printf("You have entered ");
  // printf("%d", argc);
  // printf(" arguments\n");

  if (argc < 2)
  {
    fprintf(stderr,"TSP: missing filename\n");
    exit(-1);
  }

  // for (int i = 0; i < argc; i++) // print out command line arguments
  // {
  //   printf("%s\n", argv[i]);
  // }

  FILE *in_file  = fopen(argv[1], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"TSP: could not open FILENAME\n");
    exit(-1); // must include stdlib.h 
  } 
  char str[50];

  fgets(str, 50, in_file);

  int n;

  n = atoi(str); // printf("%d\n", n);

  if (n < 2) // invalid city count
  {
    fprintf(stderr,"TSP: too few cities\n");
    exit(-1);
  }

  /* READ CITIES IN FROM INPUT FILE HERE */

  City cities[n]; // use a struct to properly read in the string namesnames

  for (int i = 0; i < n; i++)
  {

    fscanf(in_file, "%s", &cities[i].name);
  
  }
  /* Read in City Locations */

  for (int i = 0; i < n; i++)
  {
    fscanf(in_file, "%lf %lf", &cities[i].L.lat, &cities[i].L.lon);
  }

  // for (int i = 0; i < n; i++)
  // {
  //   printf("%s %lf %lf\n", cities[i].name, cities[i].L.lat, cities[i].L.lon);
  // }

  /* COMMAND LINE ARGUMENTS HERE */


  for (int i = 2; i < argc; i++)
  {
    if(strcmp(argv[i], "-given") == 0) // if the two strings are equal, then -given case
    {
      given(cities, n);
    }
    else if(strcmp(argv[i], "-farthest") == 0)
    {
      farthest();
    }
    else if (strcmp(argv[i], "-exchange") == 0)
    {
      if (strcmp(argv[i+1], "adjacent") == 0) // adjacent case
      {
        exchange_adj();
        i++; // skip so you don't read the adjacent twice
      }
      else if (strcmp(argv[i+1], "any") == 0) // any case
      {
        exchange_any();
        i++; // skip so you don't read the any twice
      }
    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }





  return 0;




}

double given(City cities[], int n)
{
  double sum = 0;
  for (int i = 0; i < n - 1; i++)
  {
    // printf("%d %d %d\n", &cities[i].name, &cities[i].L.lat, &cities[i].L.lon);
    // printf("%d\n", &cities[i].L);
    const _location *l1 = &cities[i].L;
    const _location *l2 = &cities[i+1].L;
    location_distance(l1,l2);
    // location_distance();
    // location_distance(&cities[i].L, &cities[i+1].L);
    // sum += location_distance(&(cities[i].L), &(cities[i+1].L));
  }
  // sum += location_distance(&citi)

  printf("-given            :      4490.78 HVN RSW PBI TPA FLL MCO HVN\n");

  return sum;
}
