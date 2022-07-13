#include <stdlib.h>
#include <assert.h>

#define LENGTH 100000
#define TRIALS 10000

size_t cs223_strlen(const char *);
char *str(char ch, int copies);

int main(int argc, char **argv)
{
  char *xs = str('x', LENGTH);
  for (int i = 0; i < TRIALS; i++)
    {
      size_t l = cs223_strlen(xs);
      assert(l == LENGTH);
    }
  free(xs);

  return 0;
}

size_t cs223_strlen(const char *s)
{
  const char *p = s;
  while (*(p++) != '\0');

  return (p - s - 1);
}

char *str(char ch, int copies)
{
  char *result = malloc(sizeof(char) * (copies + 1));
  for (int i = 0; i < copies; i++)
    {
      result[i] = ch;
    }
  result[copies] = '\0';

  return result;
}

