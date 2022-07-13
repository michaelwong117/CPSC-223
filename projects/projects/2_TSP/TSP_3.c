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


double given(City cities[], int n);

double farthest(City cities[], int n);

double exchange_adj(City cities[], int n);

double exchange_any(City cities[], int n);


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
  for (int i = 0; i < n; i++)
  {
    Adjacent[i] = cities[i];
    AnySwap[i] = cities[i];
    printf("%d: %s ", i, Adjacent[i].name);
  }
  printf("\n");

  // for (int i = 0; i < n; i++)
  // {
  //   printf("%s %lf %lf\n", cities[i].name, cities[i].L.lat, cities[i].L.lon);
  // }

  /* COMMAND LINE ARGUMENTS HERE */


  for (int i = 2; i < argc; i++)
  {
    if(strcmp(argv[i], "-given") == 0) // if the two strings are equal, then -given case
    {
      printf("-given            :      %.2f ", given(cities, n));
      for (int i = 0; i < n; i++)
      {
        printf("%s ", cities[i].name);
      }
      printf("%s", cities[0].name);
      printf("\n");
    }
    else if(strcmp(argv[i], "-farthest") == 0)
    {
      printf("-farthest         :      %.2f ", farthest(cities, n));
      for (int i = 0; i <= n; i++) // already included the first city at the end
      {
        printf("%s ", Farthest[i].name);
      }
      printf("\n");
    }
      
    else if (strcmp(argv[i], "-exchange") == 0)
    {
      if (strcmp(argv[i+1], "adjacent") == 0) // adjacent case
      {
        printf("-exchange adjacent:      %.2f ", exchange_adj(cities, n));

        for (int i = 0; i <= n; i++) // already included the first city at the end
        {
          printf("%s ", Adjacent[i].name);
        }
        printf("\n");

        i++; // skip so you don't read the adjacent twice
      }
      else if (strcmp(argv[i+1], "any") == 0) // any case
      {
        // exchange_any();
        i++; // skip so you don't read the any twice
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

  // printf("%s\n", Farthest[0].name);
  // strcpy(Farthest[1].name, cities[1].name);
  // // printf("%s\n", cities[1].name);
  // printf("%s\n", Farthest[1].name);
  // printf("%s\n", Farthest[0].name);
  // strcpy(Farthest[2].name, cities[2].name);
  // printf("%s\n", Farthest[0].name);


  // strcpy(Farthest[0].name, cities[0].name);
  // printf("%s\n", Farthest[0].name);
  // printf("%s\n", Farthest[1].name);
  // printf("%s\n", Farthest[2].name);


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
    // printf("%s %lf %lf\n", cities[i].name, cities[i].L.lat, cities[i].L.lon);
    // printf("%d\n", &cities[i].L);

    location *l1 = &cities[i].L;
    location *l2 = &cities[i+1].L;

    // location_distance(l1,l2);
    // location_distance();
    // location_distance(&cities[i].L, &cities[i+1].L);
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

  // City *order = malloc((n +1) *sizeof City);

  double sum = 0;

  Farthest[0] = cities[0];

  // printf("%s \n", Farthest[0].name);

  int done = 1; 

  seen[0] = 1;

  int left = 1, right = n-1;

  City curr = cities[0]; // start from the first city

  while (done < n)
  {
    // printf("%d\n", done);

    double max_dist = -1000;
    int max_idx;

    City farthest_city;

    for (int i = 0; i < n; i++)
    {
      if (seen[i] == 0)
      {
        // printf("i = %d\n", i);
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
      // strcpy(Farthest[left].name, farthest_city.name);
      // Farthest[left].L = farthest_city.L;
      // printf("Front case: Farthest idx %d has name %s\n", left, farthest_city.name);
      left++;
      // printf("%s \n", Farthest[0].name);
    }
    else // Put city in the back of the array
    {
      Farthest[right] = farthest_city;
      // strcpy(Farthest[right].name, farthest_city.name);
      // printf("Back case: Farthest idx %d has name %s\n", right, farthest_city.name);
      right--;
      // printf("%s \n", Farthest[0].name);
    }
    
    // printf("%s %s\n", curr.name, farthest_city.name);

    // sum += max_dist;
    curr = farthest_city;
    seen[max_idx] = 1;
    // printf("seen %d\n", max_idx);

    done++;

    // 
  }

  Farthest[n] = cities[0];
  // printf("Farthest Order: \n");
  for (int i = 0; i < n - 1; i++)
  {
    // printf("%s %s \n", Farthest[i].name, Farthest[i+1].name);
    sum += location_distance(&Farthest[i].L, &Farthest[i+1].L);
  }
  sum += location_distance(&Farthest[n-1].L, &Farthest[0].L);
  // sum += location_distance(&cities[n-1].L, &cities[0].L);
  // printf("%s \n", Farthest[0].name);

  F = sum;

  return sum;

  // printf("-farthest         :      4089.15 HVN MCO TPA PBI FLL RSW HVN\n");

}

double exchange_adj(City cities[], int n)
{
  if (Adj != -1)
  {
    return Adj;
  }

  double sum = 0;

  double lowest_dist = 0;
  int swap1;
  int swap2;

  double curr_back;
  double curr_forw;
  double swap_back;
  double swap_forw;

  while (lowest_dist != -1)
  {
    lowest_dist = -1;
    // swapping the last and first elements case
    
    curr_back = location_distance(&Adjacent[n-2].L, &Adjacent[n-1].L);
    curr_forw = location_distance(&Adjacent[0].L, &Adjacent[1].L);

    printf("Current Last and First: %s %s %lf ", Adjacent[n-2].name, Adjacent[n-1].name, curr_back);
    printf("%s %s %lf\n", Adjacent[0].name, Adjacent[1].name, curr_forw);

    swap_back = location_distance(&Adjacent[n-2].L, &Adjacent[0].L);
    swap_forw = location_distance(&Adjacent[n-1].L, &Adjacent[1].L);

    printf("Swap: %s %s %lf ", Adjacent[n-2].name, Adjacent[0].name, swap_back);
    printf("%s %s %lf \n", Adjacent[n-1].name, Adjacent[1].name, swap_forw);

    if (swap_back + swap_forw < curr_back + curr_forw)
    {
      lowest_dist = (curr_back + curr_forw) - (swap_back + swap_forw);
      swap1 = n-1;
      swap2 = 0;
    } 

    // swapping the first and second elements case

    curr_back = location_distance(&Adjacent[n-1].L, &Adjacent[0].L);
    curr_forw = location_distance(&Adjacent[1].L, &Adjacent[2].L);

    printf("Current First and Second: %s %s %lf ", Adjacent[n-1].name, Adjacent[0].name, curr_back);
    printf("%s %s %lf\n", Adjacent[1].name, Adjacent[2].name, curr_forw);

    swap_back = location_distance(&Adjacent[n-1].L, &Adjacent[1].L);
    swap_forw = location_distance(&Adjacent[0].L, &Adjacent[2].L);

    printf("Swap: %s %s %lf ", Adjacent[n-1].name, Adjacent[1].name, swap_back);
    printf("%s %s %lf \n", Adjacent[0].name, Adjacent[2].name, swap_forw);


    if (swap_back + swap_forw < curr_back + curr_forw)
    {
      lowest_dist = (curr_back + curr_forw) - (swap_back + swap_forw);
      swap1 = 0;
      swap2 = 1;

    } 

    // swapping the second and second to last elements case

    curr_back = location_distance(&Adjacent[n-3].L, &Adjacent[n-2].L);
    curr_forw = location_distance(&Adjacent[n-1].L, &Adjacent[0].L);

    swap_back = location_distance(&Adjacent[n-3].L, &Adjacent[n-1].L);
    swap_forw = location_distance(&Adjacent[n-2].L, &Adjacent[0].L);

    if (swap_back + swap_forw < curr_back + curr_forw)
    {
      lowest_dist = (curr_back + curr_forw) - (swap_back + swap_forw);
      swap1 = n-2;
      swap2 = n-1;
      printf("hi\n");

    } 
    for (int i = 1; i < n - 2; i++)
    {
      // elements to be swapped are Adjacent[i], Adjacent[i+1]
      curr_back = location_distance(&Adjacent[i-1].L, &Adjacent[i].L);
      curr_forw = location_distance(&Adjacent[i+1].L, &Adjacent[i+2].L);

      swap_back = location_distance(&Adjacent[i-1].L, &Adjacent[i+1].L);
      swap_forw = location_distance(&Adjacent[i].L, &Adjacent[i+1].L);

      if (swap_back + swap_forw < curr_back + curr_forw)
      {
        lowest_dist = (curr_back + curr_forw) - (swap_back + swap_forw);
        swap1 = i;
        swap2 = i+1;
        printf("hi\n");

      } 
    }
    if (lowest_dist != -1)
    {
      City temp = Adjacent[swap2];
      Adjacent[swap2] = Adjacent[swap1];
      Adjacent[swap1] = temp;
    }
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
      afterstart[i] = Adjacent[i]; 
      start = i;
    }
    else
    {
      beforestart[i] = Adjacent[i];
    }
  }
  for (int i = start; i < n; i++)
  {
    printf("Adj at %d has value of %s ", i - start, afterstart[i].name);
    Adjacent[i - start] = afterstart[i];
  }
  printf("\n");

  for (int i = 0; i < start; i++)
  {
    printf("Adj at %d has value of %s ", i + (n - start), beforestart[i].name);
    Adjacent[i + (n - start)] = beforestart[i];
  }
  printf("\n");
 
  free(beforestart);
  free(afterstart);

  // last stop is the beginning
  Adjacent[n] = Adjacent[0];

  for (int i = 0; i < n; i++)
  {
    sum += location_distance(&Adjacent[i].L, &Adjacent[i+1].L);
    printf("%s %s \n", Adjacent[i].name, Adjacent[i+1].name);
  }

  Adj = sum;
  return sum;

}
double exchange_any(City cities[], int n)
{
  printf("-exchange any     :      3907.75 HVN PBI FLL RSW TPA MCO HVN\n");
}





