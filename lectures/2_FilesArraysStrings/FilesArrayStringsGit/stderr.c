#include <stdio.h>

/**
 * A simple example of working with files.
 */

int main()
{
  int x, y;

  // open file
  FILE* infile;
  infile = fopen("numbers.txt", "r");

  // check that open was successful
  if (!infile)
    {
      // error message to stderr instead of stdout
      fprintf(stderr, "Could not open file\n");
      return 1;
    }

  // read from file
  fscanf(infile, "%d %d", &x, &y);

  // output result
  fprintf(stdout, "%d\n", x + y);

  // close files when done
  fclose(infile);
  
  return 0;
}
