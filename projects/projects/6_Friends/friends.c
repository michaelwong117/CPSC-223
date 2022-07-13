#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "graph.h"
#include "gmap.h"
#include "string_key.h"
#include "gqueue.h"

// int cmpstr(void const *a, void const *b) { 
//     char const *aa = (char const *)a;
//     char const *bb = (char const *)b;

//     return strcmp(aa, bb);
// }

int cmpstr(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
} 

int main(int argc, char*argv[])
{ 
  // argv[1] is round_robin_3.in, argv[2] is win/score, argv[3]...argv[] 
  if (argc < 2)
  {
    fprintf(stderr,"missing files\n");
    exit(-1);
  }
  if (argc < 3)
  {
    fprintf(stderr,"missing files or designator\n");
    exit(-1);
  }

  int designator = -1;

  if (strcmp(argv[2], "distance") == 0)
  {
    designator = 0;
  }
  else if (strcmp(argv[2], "mutual") == 0)
  {
    designator = 1;
  } 
  else if (strcmp(argv[2], "both") == 0)
  {
    designator = 2;
  }
  else
  {
    fprintf(stderr,"invalid designation \n");
    exit(-1);
  }

  FILE *in_file  = fopen(argv[1], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"Could not open %s\n", argv[1]);
    exit(-1); // must include stdlib.h 
  } 
  
  /** More specifically, the relevant file, here referred to as inputs, 
    * will contain a number of lines each with two non-empty strings of characters,
    *  or names, separated by a single comma. Each name represents a vertex of the 
    * graph (a person in the social network), and each line represents an edge 
    * ("friendship"). Each edge (that is, vertex-vertex combination, not permutation) 
    * is guaranteed to be unique, and each name is guaranteed to consist of 20 or fewer characters. 
    * Edges in inputs cannot extend from a vertex to itself.
    * Both input sources will be terminated by a newline character, 
    * and there will not be any other whitespace throughout, aside 
    * from other implicitly necessary newline characters. 
    * Commas will not appear in any names.
    * Guranteed no commas, no whitespace, unique edges, 20 or fewer characters
  **/

  size_t edges = 0;

  char in = getc(in_file);

  while (in != EOF)
  {
    if (in == '\n')
    {
      edges++;
    }
    else if (in == '\t' || in == '\v' || in == '\f' || in == '\r' || in == ' ')
    {
      fprintf(stderr, "Invalid whitespace\n");
      fflush(in_file);
      fclose(in_file);
      exit(-1);
    }
    in = getc(in_file);
  }
  fflush(in_file);
  fclose(in_file);

//   printf("# of edges: %ld\n", edges);

  undigraph *g = undigraph_create();

  // reading in the edges now

  in_file = fopen(argv[1], "r");

  // use getc over fscanf
 
  while ((in = getc(in_file)) != EOF)
  {
    char temp1[20];

    int len = 0;

    while (in != ',')
    {
        if (isspace(in) || len > 20 || in == EOF)
        {
            fprintf(stderr, "Invalid character in edges\n");
            undigraph_destroy(g);
            fflush(in_file);
            fclose(in_file);
            exit(-1); 
        }
        temp1[len] = in;
        in = getc(in_file);
        len++;
    }

    char *vertex1 = malloc(sizeof(char) * (len + 1));

    if (vertex1 == NULL)
    {
        undigraph_destroy(g);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
    }

    for (int i = 0; i < len; i++)
    {
        vertex1[i] = temp1[i];
    }
    vertex1[len] = '\0';
    char temp2[20];

    len = 0;

    in = getc(in_file);

    while (in != '\n')
    {
      if (isspace(in) || len > 20 || in == EOF || in == ',')
      {
        fprintf(stderr, "Invalid character in edges\n");
        free(vertex1);
        undigraph_destroy(g);
        fflush(in_file);
        fclose(in_file);
        exit(-1);
      }
      temp2[len] = in;
      in = getc(in_file);
      len++; 
    }
    char *vertex2 = malloc(sizeof(char) * (len + 1));
    if (vertex2 == NULL)
    {
        free(vertex1);
        undigraph_destroy(g);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
    }
    for (int i = 0; i < len; i++)
    {
        vertex2[i] = temp2[i];
    } 
    vertex2[len] = '\0';
    if (strcmp(vertex1, vertex2) == 0) // edge pointing to itself
    {
        free(vertex1); free(vertex2);
        undigraph_destroy(g);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
    }
    // printf("edge %s to edge %s\n", vertex1, vertex2);
    undigraph_add_edge(g, vertex1, vertex2);
    free(vertex1);
    free(vertex2);
  }

  fflush(in_file);
  fclose(in_file);

  /**
   * Each query will be supplied from standard input in the same 
   * manner as name pairs above (comma-delimited, each less than or equal to 
   * 20 characters in length), giving names of people on which one (or both) 
   * of the two tasks are to be performed. Each name must also be 
   * present in inputs. Both input sources will be terminated by a 
   * newline character, and there will not be any other whitespace throughout, 
   * aside from other implicitly necessary newline characters.
   * Commas will not appear in any names.
  */

  // use getc over fscanf

  while ((in = getc(stdin)) != EOF)
  {
    char temp1[20];

    int len = 0;

    while (in != ',')
    {
        if (isspace(in) || len > 20 || in == EOF)
        {
            fprintf(stderr, "Invalid character in edges\n");
            undigraph_destroy(g);
            exit(-1); 
        }
        temp1[len] = in;
        in = getc(stdin);
        len++;
    }

    char *vertex1 = malloc(sizeof(char) * (len + 1));

    if (vertex1 == NULL)
    {
        undigraph_destroy(g);
        exit(-1); 
    }

    for (int i = 0; i < len; i++)
    {
        vertex1[i] = temp1[i];
    }
    vertex1[len] = '\0';
    char temp2[20];

    len = 0;

    in = getc(stdin);

    while (in != '\n')
    {
      if (isspace(in) || len > 20 || in == EOF || in == ',')
      {
        fprintf(stderr, "Invalid character in edges\n");
        free(vertex1);
        undigraph_destroy(g);
        exit(-1);
      }
      temp2[len] = in;
      in = getc(stdin);
      len++; 
    }
    char *vertex2 = malloc(sizeof(char) * (len + 1));
    if (vertex2 == NULL)
    {
        free(vertex1);
        undigraph_destroy(g);
        exit(-1); 
    }
    for (int i = 0; i < len; i++)
    {
        vertex2[i] = temp2[i];
    } 
    vertex2[len] = '\0';
    if (!undigraph_has_vertex(g, vertex1) || !undigraph_has_vertex(g, vertex2))
    {
      free(vertex1); free(vertex2);
      undigraph_destroy(g);
      exit(-1);
    }
    // printf("query edge %s to edge %s\n", vertex1, vertex2);
    if (designator == 0 || designator == 2) // distance or both case
    {
      size_t distance = undigraph_bfs_distance(g, vertex1, vertex2);
      if (distance == 0)
      {
        if (strcmp(vertex1,vertex2) == 0)
        {
          printf("%s, %s are the same person\n", vertex1, vertex2);
        }
        else
        {
          printf("%s, %s are not connected\n", vertex1, vertex2);
        }
      }
      else
      {
        printf("%s, %s have minimum path length %ld\n", vertex1, vertex2, distance);
      }
    }
    if (designator == 1 || designator == 2) // mutual or both case
    {
      size_t count = 0;
      bool success = false;
      char **common = undigraph_common_neighbors(g, vertex1, vertex2, &count, &success);

      if (common == NULL && success == true)
      {
        printf("%s, %s have no mutual friends\n", vertex1, vertex2);
      }
      else if (common == NULL && success == false) // memory alloc error
      {
        free(vertex1); free(vertex2);
        undigraph_destroy(g);
        exit(-1);
      }
      else
      {
        qsort(common, count, sizeof(char *), cmpstr);
        printf("%s, %s have mutual friends", vertex1, vertex2);
        for (int i = 0; i < count; i++)
        {
          printf(" %s", common[i]);
        }
        printf("\n");
      }
      for (int i = 0; i < count; i++)
      {
        free(common[i]);
      }
      free(common);
    }
    free(vertex1);
    free(vertex2);
  }

  undigraph_destroy(g);

}