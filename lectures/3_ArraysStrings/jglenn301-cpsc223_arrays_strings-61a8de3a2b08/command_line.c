#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  // compute the sum of the lengths of the arguments
  size_t total = 0;
  for (int i = 1; i < argc; i++)
    {
      total += strlen(argv[i]);
    }

  // allocate space for concatenated string
  char *concat = malloc(sizeof(char) * (total + 1));
  if (concat == NULL)
    {
      // allocation failed
      fprintf(stderr, "%s: memory allocation failed\n", argv[0]);
      return 1;
    }

  // initialize string to empty
  concat[0] = '\0';

  // concatenate each string to end of one we're building
  // (think about what each strcat does and whether there is a better way)1
  for (int i = 1; i < argc; i++)
    {
      strcat(concat, argv[i]);
    }

  // output the result
  printf("%s\n", concat);

  free(concat);

  return 0;
}
