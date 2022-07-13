struct _gmap
{
  gmap_node **table;        // an array of pointers to heads of chains
  size_t size;         // the number of (key, value) pairs in the map
  size_t num_chains;   // the number of chains in the hash table
  size_t (*hash)(const void *);
  int (*compare)(const void *, const void *);
  void *(*copy)(const void *);
  void (*free)(void *);

};

/* What's the difference between these two?? */

// initialize the table
result->table = malloc(sizeof(gmap_node *) * GMAP_INITIAL_CAPACITY);


// add to table
gmap_node *n = malloc(sizeof(gmap_node));


p (char *) curr->key
$17 = 0x409650 "NBO"

p *(int *) curr->value

valgrind --leak-check=full --show-leak-kinds=all ./GmapUnit 1

./Blotto round_robin_3.in score 1 2 3 4

gdb ./Blotto

run round_robin_3.in win 1 2 3 4 < example_3.in 

valgrind --leak-check=full --show-leak-kinds=all ./Blotto round_robin_3.in win 1 2 3 4 < example_3.in
./Blotto round_robin_3.in win 1 2 3 4 < example_3.in


free_distributions debugging

// printf("key is %s\n", key);
// printf("values before free: ");
// for (int i = 0; i < 4; i++)
// {
//   printf("%d ", ((struct Pinfo *) value)->distribution[i]);
// }
// printf("\n");

// printf("freeing %p now!\n", ((struct Pinfo *) value)->distribution);

free(( (struct Pinfo *) value)->distribution);
// printf("just freed %p\n",  ((struct Pinfo *) value)->distribution);

// printf("values after free: ");
// for (int i = 0; i < 4; i++)
// {
//   printf("%d ", ((struct Pinfo *) value)->distribution[i]);
// }
// printf("\n");
free(value);
// ( (struct Pinfo *) value)->distribution = NULL;
// need to free values as well

// based on this, but value here is the Pinfo struct

// void gmap_unit_free_value(const void *key, void *value, void *arg)
// {
//   free(value);
// }

  /**
   * Standard input will contain one entry per line, 
   * where each entry contains a unique non-empty entry id 
   * (a string of at most 31 characters containing no commas or whitespace) 
   * and the distribution of units given as nonnegative integers in a format 
   * readable by atoi. There will be one integer per battlefield, and the sum 
   * of the integers for each entry will be the same. All parts of an entry will be separated 
   * by commas, with no whitespace except for the newline at the end of the line. 
   * You can use the entry module in /c/cs223/hw4/Optional to read entries, 
   * but we do not guarantee that the entry_read function in that 
   * module will handle all invalid inputs without errors. 
   * If you modify the module then you must submit your modifications, 
   * otherwise you can omit it from your submission.
  */

   // Pinfo *p1 = gmap_get(playersinfo, "P1");
  // Pinfo *p2 = gmap_get(playersinfo, "P2");
  // Pinfo *p3 = gmap_get(playersinfo, "P3");

  // printf("%s points: %f %s points: %f %s points: %f\n", "P1", p1->points, "P2", p2->points, "P3", p3->points);
  // printf("%s wins: %f %s wins: %f %s wins: %f\n", "P1", p1->wins / p1->num_games, "P2", p2->wins / p2->num_games, "P3", p3->wins / p3->num_games);

  // printf("%p\n", ( (struct Pinfo *) gmap_get(playersinfo, "P1")));
  // printf("%p\n", ( (struct Pinfo *) gmap_get(playersinfo, "P2")));
  // printf("%p\n", ( (struct Pinfo *) gmap_get(playersinfo, "P3")));


    // if (strcmp(player1, "entry15") == 0 || strcmp(player2, "entry15") == 0)
  // {
  //   printf("%s points: %f %s points: %f\n", player1, p1_currpoints, player2, p2_currpoints);
  // }

    // if (strcmp(player1, "entry15") == 0 || strcmp(player2, "entry15") == 0)
    // {
    //   printf("%s: %d %s: %d\n", player1, p1->distribution[i], player2, p2->distribution[i]);
    // }


    // printf("info dist address: %p\n", info->distribution);
    // printf("e dist address: %p\n", e.distribution);

    // printf("overall info address %p for key %s\n", info, e.id);

    // for (int i = 0; i < num_battlefields; i++)
    // {
    //   printf("%d ", e.distribution[i]);
    // }
    // printf("\n");


    // printf("%s\n", e.id);
    // printf("%p\n", ( (struct Pinfo *) gmap_get(playersinfo, e.id))->distribution);