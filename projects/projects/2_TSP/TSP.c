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


double G = -1, F = -1, Adj = -1, Any = -1; // Adj and Any are equivalent: one is bubble sort, one is selection,
                                           // but both arrive at the same order and total distance
City Farthest[10000]; 

City Adjacent[10000];

City AnySwap[10000];

int seen[10000];

/*
 * Go through one tour in the given order and calculate the total distance
 *
 * the output is the total distance of the tour created, the order of citites is stored in an array
 *
 */

/**
 *
 * @param cities an array of Cities, each one a struct City that has a location L and a three-character name
 * @param n the number of Cities in the input
 */

double given(City cities[], int n);

/*
 * Go through one tour according to the farthest order stated in the problem and calculate the total distance
 *
 * the output is the total distance of the tour created, the order of cities is stored in an array
 */

/**
 *
 * @param cities an array of Cities, each one a struct City that has a location L and a three-character name
 * @param n the number of Cities in the input
 */

double farthest(City cities[], int n);

/*
 * Go through one tour according to the exchange adjacent order stated in the problem and calculate the total distance
 * 
 * Handle special cases manually (ex: going from last city back to first one)
 *
 * the output is the total distance of the tour created, the order of cities is stored in an array
 */

/**
 *
 * @param cities an array of Cities, each one a struct City that has a location L and a three-character name
 * @param n the number of Cities in the input
 */

double exchange_adj(City cities[], int n);

/*
 * Go through one tour according to the exchange any order stated in the problem and calculate the total distance
 * 
 * Handle special cases manually, including all of the cases in exchange adjacent, but also the new cases
 * that arise (ex: swapping the first element and a random element i)
 *
 * the output is the total distance of the tour created, the order of cities is stored in an array
 */

/**
 *
 * @param cities an array of Cities, each one a struct City that has a location L and a three-character name
 * @param n the number of Cities in the input
 */

double exchange_any(City cities[], int n);


void reverseArray(City arr[], int start, int end)
{
    while (start < end)
    {
        City temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}    


int main(int argc, char *argv[]) 
{

  if (argc < 2)
  {
    fprintf(stderr,"TSP: missing filename\n");
    exit(-1);
  }

  FILE *in_file  = fopen(argv[1], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"TSP: could not open %s\n", argv[1]);
    exit(-1); // must include stdlib.h 
  } 
  char str[50];

  fgets(str, 50, in_file);

  int n;

  n = atoi(str); 

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
  for (int i = 0; i < n; i++)
  {
    Adjacent[i] = cities[i];
    AnySwap[i] = cities[i];
  }

  /* COMMAND LINE ARGUMENTS HERE */


  for (int i = 2; i < argc; i++)
  {
    if(strcmp(argv[i], "-given") == 0) // if the two strings are equal, then -given case
    {
      printf("-given            :%13.2f ", given(cities, n));
      for (int i = 0; i < n; i++)
      {
        printf("%s ", cities[i].name);
      }
      printf("%s", cities[0].name);
      printf("\n");
    }
    else if(strcmp(argv[i], "-farthest") == 0)
    {
      printf("-farthest         :%13.2f ", farthest(cities, n));
      for (int i = 0; i < n; i++) // already included the first city at the end
      {
        printf("%s ", Farthest[i].name);
      }
      printf("%s", Farthest[n].name);
      printf("\n");
    }
      
    else if (strcmp(argv[i], "-exchange") == 0)
    {
      if (i+1 >= argc)
      {
        fprintf(stderr,"TSP: invalid algorithm arguments\n");
        exit(-1);
      }
      if (strcmp(argv[i+1], "adjacent") == 0) // adjacent case
      {
        printf("-exchange adjacent:%13.2f ", exchange_adj(cities, n));

        for (int i = 0; i < n; i++) // already included the first city at the end
        {
          printf("%s ", Adjacent[i].name);
        }
        printf("%s", Adjacent[n].name);
        printf("\n");

        i++; // skip so you don't read the adjacent twice
      }
      else if (strcmp(argv[i+1], "any") == 0) // any case
      {
        printf("-exchange any     :%13.2f ", exchange_any(cities, n));

        for (int i = 0; i < n; i++) // already included the first city at the end
        {
          printf("%s ", AnySwap[i].name);
        }
        printf("%s", AnySwap[n].name);
        printf("\n");

        i++; // skip so you don't read the any twice
      }
      else{
        fprintf(stderr,"TSP: invalid algorithm arguments\n");
        exit(-1);
      }
    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }

  // Don't use a string of arrays if you're going to try to set individual
  // Elements of those arrays to different things
  // Strcpy will bleed over


  return 0;

}

double given(City cities[], int n)
{
  if (G != -1) // the algorithm has already been run
  {
    return G;
  }

  double sum = 0;

  for (int i = 0; i < n - 1; i++)
  {

    location *l1 = &cities[i].L;
    location *l2 = &cities[i+1].L;

    sum += location_distance(l1, l2);
  }

  sum += location_distance(&cities[n-1].L, &cities[0].L);

  G = sum;

  return sum;
}

double farthest(City cities[], int n)
{
  if (F != -1)
  {
    return F;
  }

  double sum = 0;

  Farthest[0] = cities[0];

  int done = 1; 

  seen[0] = 1;

  int left = 1, right = n-1;

  City curr = cities[0]; // start from the first city

  while (done < n)
  {

    double max_dist = -1000;
    int max_idx;

    City farthest_city;

    for (int i = 0; i < n; i++)
    {
      if (seen[i] == 0)
      {
        double dist = location_distance(&curr.L, &cities[i].L);
        if (dist > max_dist)
        {
          max_dist = dist;
          farthest_city = cities[i];
          max_idx = i;
        }
      }
    }
    if (done % 2 == 0) // Put city in the front of the array
    {
      Farthest[left] = farthest_city;
      left++;
    }
    else // Put city in the back of the array
    {
      Farthest[right] = farthest_city;
      right--;
    }
    
    curr = farthest_city;
    seen[max_idx] = 1;

    done++;

  }

  Farthest[n] = cities[0];

  for (int i = 0; i < n - 1; i++)
  {

    sum += location_distance(&Farthest[i].L, &Farthest[i+1].L);

  }
  sum += location_distance(&Farthest[n-1].L, &Farthest[0].L);

  F = sum;

  return sum;

}

double exchange_adj(City cities[], int n)
{
  if (Adj != -1)
  {
    return Adj;
  }

  int nolower = 0;

  int s1;
  int s2; // indexes of the ones you will swap

  double currBack, currForw;
  double swapBack, swapForw;

  double currDist, swapDist;



  while (nolower != 1)
  {
    nolower = 1;
    double biggestDec = -1;

    // last and first elements swap

    // the values that matter here are the curr dist between last-1 and last
    currBack = location_distance(&Adjacent[n-2].L, &Adjacent[n-1].L); 

    // the values that matter here are the curr dist between the first and first+1
    currForw = location_distance(&Adjacent[0].L, &Adjacent[1].L);

    // the values that matter here are the new, swapped dist between last-1 and last
    swapBack = location_distance(&Adjacent[n-2].L, &Adjacent[0].L);
    // the values that matter here are the new, swapped dist between first and first+1
    swapForw = location_distance(&Adjacent[n-1].L, &Adjacent[1].L);

    currDist = currBack + currForw; swapDist = swapBack + swapForw;


    /* Debugging Statements, throughout program */

    // printf("Current Last and First: %s %s %lf ", Adjacent[n-2].name, Adjacent[n-1].name, currBack);
    // printf("%s %s %lf\n", Adjacent[0].name, Adjacent[1].name, currForw);

    // printf("Swap: %s %s %lf ", Adjacent[n-2].name, Adjacent[0].name, swapBack);
    // printf("%s %s %lf \n", Adjacent[n-1].name, Adjacent[1].name, swapForw);

    if ( swapDist < currDist )
    {
      if (biggestDec < currDist - swapDist)
      {
        biggestDec = currDist - swapDist;
        nolower = 0; // found a swap that lowers the total distances
        s1 = n-1; 
        s2 = 0;
      }
    }


    // First and Second Elements Swap

    currBack = location_distance(&Adjacent[n-1].L, &Adjacent[0].L);
    currForw = location_distance(&Adjacent[1].L, &Adjacent[2].L);


    // printf("Current First and Second: %s %s %lf ", Adjacent[n-1].name, Adjacent[0].name, currBack);
    // printf("%s %s %lf\n", Adjacent[1].name, Adjacent[2].name, currForw);


    swapBack = location_distance(&Adjacent[n-1].L, &Adjacent[1].L);
    swapForw = location_distance(&Adjacent[0].L, &Adjacent[2].L);

    // printf("Swap: %s %s %lf ", Adjacent[n-1].name, Adjacent[1].name, swapBack);
    // printf("%s %s %lf \n", Adjacent[0].name, Adjacent[2].name, swapForw);

    currDist = currBack + currForw; swapDist = swapBack + swapForw;

    if ( swapDist < currDist )
    {
      if (biggestDec < currDist - swapDist)
      {
        biggestDec = currDist - swapDist;
        nolower = 0; // found a swap that lowers the total distances
        s1 = 0; 
        s2 = 1;
      }
    }

    // Last and Second Last Elements Swap

    if (n > 2)
    {

      currBack = location_distance(&Adjacent[n-3].L, &Adjacent[n-2].L);
      currForw = location_distance(&Adjacent[n-1].L, &Adjacent[0].L);

      // printf("Current Last and Second Last: %s %s %lf ", Adjacent[n-3].name, Adjacent[n-2].name, currBack);
      // printf("%s %s %lf\n", Adjacent[n-1].name, Adjacent[0].name, currForw);

      swapBack = location_distance(&Adjacent[n-3].L, &Adjacent[n-1].L);
      swapForw = location_distance(&Adjacent[n-2].L, &Adjacent[0].L);

      // printf("Swap: %s %s %lf ", Adjacent[n-3].name, Adjacent[n-1].name, swapBack);
      // printf("%s %s %lf \n", Adjacent[n-2].name, Adjacent[0].name, swapForw);

      currDist = currBack + currForw; swapDist = swapBack + swapForw;

      if ( swapDist < currDist )
      {
        if (biggestDec < currDist - swapDist)
        {
          biggestDec = currDist - swapDist;
          nolower = 0; // found a swap that lowers the total distances
          s1 = n-2; 
          s2 = n-1;
        }
      }
    }

    for (int i = 1; i < n - 2; i++)
    {
      // elements to be swapped are Adjacent[i], Adjacent[i+1]
      currBack = location_distance(&Adjacent[i-1].L, &Adjacent[i].L);
      currForw = location_distance(&Adjacent[i+1].L, &Adjacent[i+2].L);

      // printf("Looking to swap %s %s:\n\n", Adjacent[i].name, Adjacent[i+1].name);
      // printf("Current: %s %s %lf ", Adjacent[i-1].name, Adjacent[i].name, currBack);
      // printf("%s %s %lf\n", Adjacent[i+1].name, Adjacent[i+2].name, currForw);

      swapBack = location_distance(&Adjacent[i-1].L, &Adjacent[i+1].L);
      swapForw = location_distance(&Adjacent[i].L, &Adjacent[i+2].L);

      // printf("Swap: %s %s %lf ", Adjacent[i-1].name, Adjacent[i+1].name, swapBack);
      // printf("%s %s %lf \n\n", Adjacent[i].name, Adjacent[i+2].name, swapForw);


      currDist = currBack + currForw; swapDist = swapBack + swapForw;

      if ( swapDist < currDist )
      {
        if (biggestDec < currDist - swapDist)
        {
          biggestDec = currDist - swapDist;
          nolower = 0; // found a swap that lowers the total distances
          s1 = i; 
          s2 = i+1;
        }
      }
    }
    if (nolower != 1) 
    {
      // printf("\nSwapping %s %s for a decrease of %lf\n", Adjacent[s1].name, Adjacent[s2].name, biggestDec);
      City temp = Adjacent[s2];
      Adjacent[s2] = Adjacent[s1];
      Adjacent[s1] = temp;
    }
    // for (int i = 0; i < n; i++){
    //   printf("%s ", Adjacent[i].name);
    // }
    // printf("\n");

  }
  // still have to make sure order starts with beginning HVN

  int start = 0;

  City *beforestart = malloc((n + 1) * sizeof (City));
  City *afterstart = malloc((n + 1) * sizeof (City));

  for (int i = 0; i < n; i++)
  {
    // printf("%s ", Adjacent[i].name);
    if (strcmp(Adjacent[i].name, cities[0].name) == 0) // Can't compare with Adjacent[i].name == cities[0].name
    {
      start = i;
    }
    if (start != 0)
    {
      afterstart[i] = Adjacent[i]; 
      // printf("afterstart %s\n", Adjacent[i].name);
    }
    else
    {
      beforestart[i] = Adjacent[i];
      // printf("beforestart %s\n", Adjacent[i].name);
    }
  }
  if (start != 0)
  {
    for (int i = start; i < n; i++)
    {
      // printf("Adj after at %d has value of %s ", i - start, afterstart[i].name);
      Adjacent[i - start] = afterstart[i];
    }
    // printf("\n");

    for (int i = 0; i < start; i++)
    {
      // printf("Adj before at %d has value of %s ", i + (n - start), beforestart[i].name);
      Adjacent[i + (n - start)] = beforestart[i];
    }
    // printf("\n");
    
    free(beforestart);
    free(afterstart);
  }


  // last stop is the beginning
  Adjacent[n] = Adjacent[0];

  // Reverse Array Case

  int secondCity;
  int penultimateCity;

  for (int i = 0; i < n; i++)
  {
    if (strcmp(cities[i].name, Adjacent[1].name) == 0) // index of the second TOUR city IN the input
    {
      // printf("Second tour in Input: %s %d\n", Adjacent[1].name, i);
      secondCity = i;
    }
    if (strcmp(cities[i].name, Adjacent[n-1].name) == 0) // index of the penultimate TOUR city IN the input
    {
      // printf("Penultimate tour in Input: %s %d\n", Adjacent[n-1].name, i);
      penultimateCity = i;
    }
  }

  if (secondCity > penultimateCity)
  {
    reverseArray(Adjacent, 0, n);
  }

  double sum = 0;

  for (int i = 0; i < n; i++)
  {
    sum += location_distance(&Adjacent[i].L, &Adjacent[i+1].L);
    // printf("%s %s \n", Adjacent[i].name, Adjacent[i+1].name);
  }

  Adj = sum;
  return sum;

}
double exchange_any(City cities[], int n)
{
  if (Any != -1)
  {
    return Any;
  }

  int nolower = 0;

  int s1;
  int s2; // indexes of the ones you will swap

  double currA, currB, currC, currD;
  double swapA, swapB, swapC, swapD;

  double currDist, swapDist;

  double currBack, currForw;
  double swapBack, swapForw;


  int count = 0;
  while (nolower != 1)
  {
    count++;
    nolower = 1;
    double biggestDec = -1;

    // printf("casework starting: \n");

    // last and first elements swap

    // the values that matter here are the curr dist between last-1 and last
    currBack = location_distance(&AnySwap[n-2].L, &AnySwap[n-1].L); 

    // the values that matter here are the curr dist between the first and first+1
    currForw = location_distance(&AnySwap[0].L, &AnySwap[1].L);

    // the values that matter here are the new, swapped dist between last-1 and last
    swapBack = location_distance(&AnySwap[n-2].L, &AnySwap[0].L);
    // the values that matter here are the new, swapped dist between first and first+1
    swapForw = location_distance(&AnySwap[n-1].L, &AnySwap[1].L);

    currDist = currBack + currForw; swapDist = swapBack + swapForw;

    // printf("Current Last and First: %s %s %lf ", AnySwap[n-2].name, AnySwap[n-1].name, currBack);
    // printf("%s %s %lf\n", AnySwap[0].name, AnySwap[1].name, currForw);

    // printf("Swap: %s %s %lf ", AnySwap[n-2].name, AnySwap[0].name, swapBack);
    // printf("%s %s %lf \n", AnySwap[n-1].name, AnySwap[1].name, swapForw);

    if ( swapDist < currDist )
    {
      if (biggestDec < currDist - swapDist)
      {
        biggestDec = currDist - swapDist;
        nolower = 0; // found a swap that lowers the total distances
        s1 = n-1; 
        s2 = 0;
      }
    }


    // First and Second Elements Swap

    currBack = location_distance(&AnySwap[n-1].L, &AnySwap[0].L);
    currForw = location_distance(&AnySwap[1].L, &AnySwap[2].L);


    // printf("Current First and Second: %s %s %lf ", AnySwap[n-1].name, AnySwap[0].name, currBack);
    // printf("%s %s %lf\n", AnySwap[1].name, AnySwap[2].name, currForw);


    swapBack = location_distance(&AnySwap[n-1].L, &AnySwap[1].L);
    swapForw = location_distance(&AnySwap[0].L, &AnySwap[2].L);

    // printf("Swap: %s %s %lf ", AnySwap[n-1].name, AnySwap[1].name, swapBack);
    // printf("%s %s %lf \n", AnySwap[0].name, AnySwap[2].name, swapForw);

    currDist = currBack + currForw; swapDist = swapBack + swapForw;

    if ( swapDist < currDist )
    {
      if (biggestDec < currDist - swapDist)
      {
        biggestDec = currDist - swapDist;
        nolower = 0; // found a swap that lowers the total distances
        s1 = 0; 
        s2 = 1;
      }
    }

    // Last and Second Last Elements Swap

    currBack = location_distance(&AnySwap[n-3].L, &AnySwap[n-2].L);
    currForw = location_distance(&AnySwap[n-1].L, &AnySwap[0].L);

    // printf("Current Last and Second Last: %s %s %lf ", AnySwap[n-3].name, AnySwap[n-2].name, currBack);
    // printf("%s %s %lf\n", AnySwap[n-1].name, AnySwap[0].name, currForw);

    swapBack = location_distance(&AnySwap[n-3].L, &AnySwap[n-1].L);
    swapForw = location_distance(&AnySwap[n-2].L, &AnySwap[0].L);

    // printf("Swap: %s %s %lf ", AnySwap[n-3].name, AnySwap[n-1].name, swapBack);
    // printf("%s %s %lf \n", AnySwap[n-2].name, AnySwap[0].name, swapForw);

    currDist = currBack + currForw; swapDist = swapBack + swapForw;

    if ( swapDist < currDist )
    {
      if (biggestDec < currDist - swapDist)
      {
        biggestDec = currDist - swapDist;
        nolower = 0; // found a swap that lowers the total distances
        s1 = n-2; 
        s2 = n-1;
      }
    }

    // first element swaps with anything

    for (int i = 2; i < n - 1; i++)
    {

      // printf("Looking to swap %s %s:\n\n", AnySwap[0].name, AnySwap[i].name);
      currA = location_distance(&AnySwap[0].L, &AnySwap[1].L);
      currB = location_distance(&AnySwap[n-1].L, &AnySwap[0].L);
      currC = location_distance(&AnySwap[i].L, &AnySwap[i+1].L);
      currD = location_distance(&AnySwap[i-1].L, &AnySwap[i].L);

      // printf("Current Comparisons: %s %s %lf \n", AnySwap[0].name, AnySwap[1].name, currA);
      // printf("Current Comparisons: %s %s %lf \n", AnySwap[n-1].name, AnySwap[0].name, currB);
      // printf("Current Comparisons: %s %s %lf \n", AnySwap[i].name, AnySwap[i+1].name, currC);
      // printf("Current Comparisons: %s %s %lf \n\n", AnySwap[i-1].name, AnySwap[i].name, currD);

      swapA = location_distance(&AnySwap[i].L, &AnySwap[1].L);
      swapB = location_distance(&AnySwap[n-1].L, &AnySwap[i].L);
      swapC = location_distance(&AnySwap[0].L, &AnySwap[i+1].L);
      swapD = location_distance(&AnySwap[i-1].L, &AnySwap[0].L);

      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[i].name, AnySwap[1].name, swapA);
      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[n-1].name, AnySwap[i].name, swapB);
      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[0].name, AnySwap[i+1].name, swapC);
      // printf("Swap Comparisons: %s %s %lf \n\n", AnySwap[i-1].name, AnySwap[0].name, swapD);


      currDist = currA+currB+currC+currD; swapDist = swapA+swapB+swapC+swapD;

      // printf("currDist: %lf swapDist: %lf\n\n", currDist, swapDist);

      if ( swapDist < currDist )
      {
        // printf("\nThis Swap would give a Decrease of %lf\n\n", currDist - swapDist);

        if (biggestDec < currDist - swapDist)
        {
          biggestDec = currDist - swapDist;
          nolower = 0; // found a swap that lowers the total distances
          s1 = 0; 
          s2 = i;
        }
      }
    }

    // last element swaps with anything

    for (int i = 1; i < n - 2; i++)
    {

      // printf("Looking to swap %s %s:\n\n", AnySwap[i].name, AnySwap[n-1].name);
      currA = location_distance(&AnySwap[i-1].L, &AnySwap[i].L);
      currB = location_distance(&AnySwap[i].L, &AnySwap[i+1].L);
      currC = location_distance(&AnySwap[n-2].L, &AnySwap[n-1].L);
      currD = location_distance(&AnySwap[n-1].L, &AnySwap[0].L);

      // printf("Current Comparisons: %s %s %lf \n", AnySwap[i-1].name, AnySwap[i].name, currA);
      // printf("Current Comparisons: %s %s %lf \n", AnySwap[i].name, AnySwap[i+1].name, currB);
      // printf("Current Comparisons: %s %s %lf \n", AnySwap[n-2].name, AnySwap[n-1].name, currC);
      // printf("Current Comparisons: %s %s %lf \n\n", AnySwap[n-1].name, AnySwap[0].name, currD);

      swapA = location_distance(&AnySwap[i-1].L, &AnySwap[n-1].L);
      swapB = location_distance(&AnySwap[n-1].L, &AnySwap[i+1].L);
      swapC = location_distance(&AnySwap[n-2].L, &AnySwap[i].L);
      swapD = location_distance(&AnySwap[i].L, &AnySwap[0].L);

      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[i-1].name, AnySwap[n-1].name, swapA);
      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[n-1].name, AnySwap[i+1].name, swapB);
      // printf("Swap Comparisons: %s %s %lf \n", AnySwap[n-2].name, AnySwap[i].name, swapC);
      // printf("Swap Comparisons: %s %s %lf \n\n", AnySwap[i].name, AnySwap[0].name, swapD);


      currDist = currA+currB+currC+currD; swapDist = swapA+swapB+swapC+swapD;

      // printf("currDist: %lf swapDist: %lf\n\n", currDist, swapDist);

      if ( swapDist < currDist )
      {
        // printf("\nThis Swap would give a Decrease of %lf\n\n", currDist - swapDist);

        if (biggestDec < currDist - swapDist)
        {
          biggestDec = currDist - swapDist;
          nolower = 0; // found a swap that lowers the total distances
          s1 = i; 
          s2 = n-1;
        }
      }
    }
    // printf("casework ending: \n");
    // Adjacent

    for (int i = 1; i < n - 2; i++)
    {
      // elements to be swapped are AnySwap[i], AnySwap[i+1]
      currBack = location_distance(&AnySwap[i-1].L, &AnySwap[i].L);
      currForw = location_distance(&AnySwap[i+1].L, &AnySwap[i+2].L);

      // printf("Looking to swap %s %s:\n\n", AnySwap[i].name, AnySwap[i+1].name);
      // printf("Current: %s %s %lf ", AnySwap[i-1].name, AnySwap[i].name, currBack);
      // printf("%s %s %lf\n", AnySwap[i+1].name, AnySwap[i+2].name, currForw);

      swapBack = location_distance(&AnySwap[i-1].L, &AnySwap[i+1].L);
      swapForw = location_distance(&AnySwap[i].L, &AnySwap[i+2].L);

      // printf("Swap: %s %s %lf ", AnySwap[i-1].name, AnySwap[i+1].name, swapBack);
      // printf("%s %s %lf \n\n", AnySwap[i].name, AnySwap[i+2].name, swapForw);


      currDist = currBack + currForw; swapDist = swapBack + swapForw;

      if ( swapDist < currDist )
      {
        if (biggestDec < currDist - swapDist)
        {
          biggestDec = currDist - swapDist;
          nolower = 0; // found a swap that lowers the total distances
          s1 = i; 
          s2 = i+1;
        }
      }
    }
    // printf("n^2 starting: \n");
    // Try ANY of these swaps
    for (int i = 1; i < n - 2; i++)
    {
      for (int j = i+2; j < n - 1; j++)
      {
        // printf("ANY: Looking to swap %s %s:\n\n", AnySwap[i].name, AnySwap[j].name);
        currA = location_distance(&AnySwap[i-1].L, &AnySwap[i].L);
        currB = location_distance(&AnySwap[i].L, &AnySwap[i+1].L);
        currC = location_distance(&AnySwap[j-1].L, &AnySwap[j].L);
        currD = location_distance(&AnySwap[j].L, &AnySwap[j+1].L);

        // printf("Current Comparisons: %s %s %lf \n", AnySwap[i-1].name, AnySwap[i].name, currA);
        // printf("Current Comparisons: %s %s %lf \n", AnySwap[i].name, AnySwap[i+1].name, currB);
        // printf("Current Comparisons: %s %s %lf \n", AnySwap[j-1].name, AnySwap[j].name, currC);
        // printf("Current Comparisons: %s %s %lf \n\n", AnySwap[j].name, AnySwap[j+1].name, currD);

        swapA = location_distance(&AnySwap[i-1].L, &AnySwap[j].L);
        swapB = location_distance(&AnySwap[j].L, &AnySwap[i+1].L);
        swapC = location_distance(&AnySwap[j-1].L, &AnySwap[i].L);
        swapD = location_distance(&AnySwap[i].L, &AnySwap[j+1].L);

        // printf("Swap Comparisons: %s %s %lf \n", AnySwap[i-1].name, AnySwap[j].name, swapA);
        // printf("Swap Comparisons: %s %s %lf \n", AnySwap[j].name, AnySwap[i+1].name, swapB);
        // printf("Swap Comparisons: %s %s %lf \n", AnySwap[j-1].name, AnySwap[i].name, swapC);
        // printf("Swap Comparisons: %s %s %lf \n\n", AnySwap[i].name, AnySwap[j+1].name, swapD);

        currDist = currA+currB+currC+currD; swapDist = swapA+swapB+swapC+swapD;

        // printf("currDist: %lf swapDist: %lf\n\n", currDist, swapDist);

        if ( swapDist < currDist )
        {
         // printf("\nThis Swap would give a Decrease of %lf\n\n", currDist - swapDist);

         if (biggestDec < currDist - swapDist)
         {
           biggestDec = currDist - swapDist;
           nolower = 0; // found a swap that lowers the total distances
           s1 = i; 
           s2 = j;
         }
        }
      }

    }
    // printf("swap count: %d\n", count);

    // printf("n^2 ending: \n");

    
    if (nolower != 1) 
    {
      // printf("\nSwapping %s %s for a decrease of %lf\n", AnySwap[s1].name, AnySwap[s2].name, biggestDec);
      City temp = AnySwap[s2];
      AnySwap[s2] = AnySwap[s1];
      AnySwap[s1] = temp;
    }
    // for (int i = 0; i < n; i++){
    //   printf("%s ", AnySwap[i].name);
    // }
    // printf("\n");

  }

    // still have to make sure order starts with beginning HVN

    int start = 0;

    City *beforestart = malloc((n + 1) * sizeof (City));
    City *afterstart = malloc((n + 1) * sizeof (City));

    for (int i = 0; i < n; i++)
    {
      // printf("%s ", AnySwap[i].name);
      if (strcmp(AnySwap[i].name, cities[0].name) == 0) // Can't compare with AnySwap[i].name == cities[0].name
      {
        start = i;
      }
      if (start != 0)
      {
        afterstart[i] = AnySwap[i]; 
        // printf("afterstart %s\n", AnySwap[i].name);
      }
      else
      {
        beforestart[i] = AnySwap[i];
        // printf("beforestart %s\n", AnySwap[i].name);
      }
    }
    if (start != 0)
    {
      for (int i = start; i < n; i++)
      {
        // printf("Adj after at %d has value of %s ", i - start, afterstart[i].name);
        AnySwap[i - start] = afterstart[i];
      }
      // printf("\n");

      for (int i = 0; i < start; i++)
      {
        // printf("Adj before at %d has value of %s ", i + (n - start), beforestart[i].name);
        AnySwap[i + (n - start)] = beforestart[i];
      }
      // printf("\n");
      
      free(beforestart);
      free(afterstart);
    }


    // last stop is the beginning
    AnySwap[n] = AnySwap[0];

    // Reverse Array Case

    int secondCity;
    int penultimateCity;

    for (int i = 0; i < n; i++)
    {
      if (strcmp(cities[i].name, AnySwap[1].name) == 0) // index of the second TOUR city IN the input
      {
        // printf("Second tour in Input: %s %d\n", AnySwap[1].name, i);
        secondCity = i;
      }
      if (strcmp(cities[i].name, AnySwap[n-1].name) == 0) // index of the penultimate TOUR city IN the input
      {
        // printf("Penultimate tour in Input: %s %d\n", AnySwap[n-1].name, i);
        penultimateCity = i;
      }
    }

    if (secondCity > penultimateCity)
    {
      reverseArray(AnySwap, 0, n);
    }

    double sum = 0;

    for (int i = 0; i < n; i++)
    {
      sum += location_distance(&AnySwap[i].L, &AnySwap[i+1].L);
      // printf("%s %s \n", AnySwap[i].name, AnySwap[i+1].name);
    }

    Any = sum;

    return sum;


  
}





