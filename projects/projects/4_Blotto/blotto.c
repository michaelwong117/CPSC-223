#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gmap.h"
#include "entry.h"
#include "gmap_test_functions.h"
#include "string_key.h"

// the value with the player as a key
typedef struct Pinfo
{
  double points; // the player's running score (in case of win, 0 or 1)
  int *distribution; // the player's score distribution
  int num_games; // to calculate averages
  double wins; // number of wins
  char *id;
} Pinfo;


// frees malloc'd distribution and the malloc'd values
void free_distributions(const void *key, void *value, void *p)
{
  if (( (struct Pinfo *) value)->distribution != NULL)
  {
    free(( (struct Pinfo *) value)->distribution);
    free(( (struct Pinfo *) value)->id);
    free(value);
  }
}

bool exitProgram = false;

void blotto_game(char *player1, char *player2, gmap *playersinfo, 
                            int num_battlefields, int *battlefield_val)
{
  if(gmap_contains_key(playersinfo, player1) == false)
  {
    exitProgram = true;
    return;
  }
  if(gmap_contains_key(playersinfo, player2) == false)
  {
    exitProgram = true;
    return;
  }

  Pinfo *p1 = gmap_get(playersinfo, player1);
  Pinfo *p2 = gmap_get(playersinfo, player2);

  double p1_currpoints = 0;
  double p2_currpoints = 0;


  for (int i = 0; i < num_battlefields; i++)
  {
    if (p1->distribution[i] > p2->distribution[i])
    {
      p1_currpoints += battlefield_val[i];
    }
    else if (p1->distribution[i] < p2->distribution[i])
    {
      p2_currpoints += battlefield_val[i];
    }
    else if (p1->distribution[i] == p2->distribution[i])
    {
      p1_currpoints += (double) battlefield_val[i] / 2;
      p2_currpoints += (double) battlefield_val[i] / 2;
    }
  }

  p1->points += p1_currpoints;
  p2->points += p2_currpoints;

  p1->num_games++;
  p2->num_games++;

  if (p1_currpoints > p2_currpoints)
  {
    p1->wins++;
  }
  else if (p1_currpoints < p2_currpoints)
  {
    p2->wins++;
  }
  else if (p1_currpoints == p2_currpoints)
  {
    p1->wins += 0.5;
    p2->wins += 0.5;
  }

}

int compare_wins(const void *a, const void *b)
{
  Pinfo **p1 = (Pinfo **)(a);
  Pinfo **p2 = (Pinfo **)(b);
  
  double p1wins = (*p1)->wins / (*p1)->num_games;
  double p2wins = (*p2)->wins / (*p2)->num_games;

  if (p1wins > p2wins)
  {
    return 1;
  }
  else if (p1wins < p2wins)
  {
    return -1;
  }
  else
  {
    return -1*strcmp((*p1)->id, (*p2)->id);
  }
}

int compare_scores(const void *a, const void *b)
{
  Pinfo **p1 = (Pinfo **)(a);
  Pinfo **p2 = (Pinfo **)(b);
  
  double p1score = (*p1)->points / (*p1)->num_games;
  double p2score = (*p2)->points / (*p2)->num_games;

  if (p1score > p2score)
  {
    return 1;
  }
  else if (p1score < p2score)
  {
    return -1;
  }
  else
  {
    return -1*strcmp((*p1)->id, (*p2)->id);
  }
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
  if (argc < 4)
  {
    fprintf(stderr,"missing files, designator, or values \n");
    exit(-1);
  }

  int designator = -1;

  if (strcmp(argv[2], "win") == 0)
  {
    designator = 0;
  }
  else if (strcmp(argv[2], "score") == 0)
  {
    designator = 1;
  } 
  else
  {
    fprintf(stderr,"invalid designation \n");
    exit(-1);
  }

  int battlefield_val[argc - 3];

  for (int i = 3; i < argc; i++)
  {
    char *end;
    long val = strtol(argv[i], &end, 10);
    if (end == argv[i] || *end != '\0' || errno == ERANGE)
    {
      fprintf(stderr,"bad integer \n");
      exit(-1);
    }
    battlefield_val[i-3] = (int) val;
  }
  /** 
   * The matchups input file will contain one matchup per line,
   * where each matchup is specified by giving the id of two different entries, 
   * separated by a single space, with no other whitespace except for the 
   * newline at the end of the line.
  */

  FILE *in_file  = fopen(argv[1], "r");

  if (in_file == NULL) 
  {   
    fprintf(stderr,"Could not open %s\n", argv[1]);
    exit(-1); // must include stdlib.h 
  } 

  size_t matchups = 0;

  char in = getc(in_file);

  while (in != EOF)
  {
    if (in == '\n')
    {
      matchups++;
    }
    else if (in == '\t' || in == '\v' || in == '\f' || in == '\r')
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

  // handled case to see if stdin is empty (no pipe or false pipe)
  // not needed, breaks program
  // if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) <= 0)
  // {
  //   fprintf(stderr, "Nothing is being piped to stdin\n");
  //   exit(-1);
  // }

  gmap *playersinfo = gmap_create(duplicate, compare_keys, java_hash_string, free);

  int num_battlefields = argc - 3;
  int max_id = 31; 
  int num_entries = 0;

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
 
  entry e = entry_read(stdin, max_id, num_battlefields);

  if (e.id == NULL)
  {
    fprintf(stderr, "Invalid entry\n");
    if (e.id != NULL) free(e.id);
    if (e.distribution != NULL) free(e.distribution);
    gmap_for_each(playersinfo, free_distributions, NULL);
    gmap_destroy(playersinfo);
    exit(-1); 
  }

  while (strcmp(e.id, "") != 0)
  {
    // Must handle bad entry cases better
    // if there is a duplicate entry exit
    if (gmap_contains_key(playersinfo, e.id) == true)
    {
      if (e.id != NULL) free(e.id);
      if (e.distribution != NULL) free(e.distribution);
      fprintf(stderr, "Duplicate entry\n");
      gmap_for_each(playersinfo, free_distributions, NULL);
      gmap_destroy(playersinfo);
      exit(-1); 
    }

    Pinfo *info = malloc(sizeof(Pinfo));
    info->points = 0;
    info->num_games = 0;
    info->wins = 0;
    info->distribution = e.distribution;
    info->id = e.id;

    for (int i = 0; i < num_battlefields; i++)
    {
      // not enough values in distribution, return error
      if (e.distribution[i] == -999999)
      {
        fprintf(stderr, "Invalid distribution\n");
        if (e.id != NULL) free(e.id);
        if (e.distribution != NULL) free(e.distribution);
        gmap_for_each(playersinfo, free_distributions, NULL);
        gmap_destroy(playersinfo);
        free(info);
        exit(-1); 
      }
    }

    gmap_put(playersinfo, e.id, info); // gmap put creates a copy of e.id
    num_entries++;

    e = entry_read(stdin, max_id, num_battlefields);

    if (e.id == NULL)
    {
      fprintf(stderr, "Invalid entry\n");
      if (e.id != NULL) free(e.id);
      if (e.distribution != NULL) free(e.distribution);
      gmap_for_each(playersinfo, free_distributions, NULL);
      gmap_destroy(playersinfo);
      exit(-1); 
    }
    // need to eventually free e.distribution and e.id with gmap for each at the end
  }
  // printf("%d\n", num_entries);
  if (e.id != NULL)
  {
    // printf("%s\n", e.id);
    free(e.id);
    free(e.distribution);
  }

  // reading in the matchup ids now

  in_file  = fopen(argv[1], "r");

  // use getc over fscanf

  /**
   * each entry contains a unique 
   * non-empty entry id (a string of at most 31 characters 
   * containing no commas or whitespace)
   * 
   * The matchups input file will contain one matchup per line, 
   * where each matchup is specified by giving the id of two 
   * different entries that were given in standard input. 
   * The two ids for each matchup will be separated by a single space, 
   * and there will be no other whitespace except 
   * for the newline at the end of each line
   * 
   * try matchups like entry1 entry2 entry3 all on the same line
   * test
  */
 
  while ((in = getc(in_file)) != EOF)
  {
    char temp1[31];

    int len = 0;

    while (in != ' ')
    {
      if (in == ',' || len >= 31 || in == EOF)
      {
        fprintf(stderr, "Invalid character in ID\n");

        gmap_for_each(playersinfo, free_distributions, NULL);
        gmap_destroy(playersinfo);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
      }

      temp1[len] = in;
      in = getc(in_file);
      len++; 
    }

    char player1[len+1];

    for (int i = 0; i < len; i++)
    {
      if(temp1[i] == ',' || temp1[i] == '\n' || temp1[i] == ' ' || temp1[i] == '\t' 
            || temp1[i] == '\v' || temp1[i] == '\f' || temp1[i] == '\r')
      {
        fprintf(stderr, "Invalid character in ID\n");
        gmap_for_each(playersinfo, free_distributions, NULL);
        gmap_destroy(playersinfo);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
      }
      player1[i] = temp1[i];
    }
    player1[len] = '\0';

    char temp2[31];

    len = 0;

    in = getc(in_file);

    while (in != '\n')
    {
      if (in == ',' || in == ' ' || len >= 31 || in == EOF)
      {
        fprintf(stderr, "Invalid character in ID\n");

        gmap_for_each(playersinfo, free_distributions, NULL);
        gmap_destroy(playersinfo);

        fflush(in_file);
        fclose(in_file);

        exit(-1);
      }
      temp2[len] = in;
      in = getc(in_file);
      len++; 
    }

    char player2[len+1];

    for (int i = 0; i < len; i++)
    {
      if(temp2[i] == ',' || temp2[i] == '\n' || temp2[i] == ' ' || temp2[i] == '\t' 
            || temp2[i] == '\v' || temp2[i] == '\f' || temp2[i] == '\r')
      {
        fprintf(stderr, "Invalid character in ID\n");
        gmap_for_each(playersinfo, free_distributions, NULL);
        gmap_destroy(playersinfo);
        fflush(in_file);
        fclose(in_file);
        exit(-1); 
      }
      player2[i] = temp2[i];
    }
    player2[len] = '\0';

    // if (player1[0] == ' ' || player2[0] == ' ')
    // {
    //   fprintf(stderr, "Invalid character in ID\n");

    //   gmap_for_each(playersinfo, free_distributions, NULL);
    //   gmap_destroy(playersinfo);

    //   fflush(in_file);
    //   fclose(in_file);

    //   exit(-1);
    // }
    // printf("%s %s\n", player1, player2);
    blotto_game(player1, player2, playersinfo, num_battlefields, battlefield_val);
    if (exitProgram == true)
    {
      fprintf(stderr, "Bad Blotto Game\n");
      fflush(in_file);
      fclose(in_file);
      gmap_for_each(playersinfo, free_distributions, NULL);
      gmap_destroy(playersinfo);
      exit(-1);
    }
  }

  fflush(in_file);
  fclose(in_file);

  char **keys = (char **) gmap_keys(playersinfo);

  if (keys == NULL) exitProgram = true; //handle edge

  if (exitProgram == true)
  {
    fprintf(stderr, "Bad keys\n");
    free(keys);
    gmap_for_each(playersinfo, free_distributions, NULL);
    gmap_destroy(playersinfo);
    exit(-1);
  }

  Pinfo **keysval = (Pinfo **) malloc(sizeof(*keysval) * num_entries);

  // the number of entries that also show up in the matchups file
  int n = 0;

  for (int i = 0; i < num_entries; i++)
  {
    Pinfo *p = gmap_get(playersinfo, keys[i]);
    // printf("for %s, wins: %f, num_games: %d\n", p->id, p->wins, p->num_games);
    if(p->num_games != 0)
    {
      keysval[n] = p; 
      n++;
    }
  }

  if(n == 0)
  {
    fprintf(stderr, "bad keysval\n");
    free(keys);
    free(keysval);
    gmap_for_each(playersinfo, free_distributions, NULL);
    gmap_destroy(playersinfo);
    exit(-1);
  }

  // printf("%d\n", n);
  if (designator == 0)
  {
    qsort(keysval, n, sizeof(char *), compare_wins);
  }
  else if (designator == 1)
  {
    qsort(keysval, n, sizeof(char *), compare_scores);
  }


  for (int i = n-1; i >= 0; i--)
  {
    if(keysval[i]->num_games == 0) continue;

    if(designator == 0)
    {
      printf("%7.3f %s\n", keysval[i]->wins / keysval[i]->num_games, keysval[i]->id);
    }
    else if(designator == 1)
    {
      printf("%7.3f %s\n", keysval[i]->points / keysval[i]->num_games, keysval[i]->id);
    }
  }

  free(keys);
  free(keysval);

  gmap_for_each(playersinfo, free_distributions, NULL);
  gmap_destroy(playersinfo);

}