#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Counts the whitespace characters in the given string.
 *
 * @param s a C-string (a null-terminated array of characters)
 * @return the number of whitespace characters in s
 */
int count_spaces(const char s[]);

/**
 * Makes the destination equal to n copies of the source concatenated
 * together.
 *
 * @param dest a C-string with enough space to hold the result
 * @param src a C-string
 * @param n a nonnegative integer
 */
void make_copies(char dest[], char src[], int n);

int main()
{
  // strings are null ('\0') terminated arrays of characters
  // size must include the null character
  char given_name[6] = {'J', 'a', 'm', 'e', 's', '\0'};

  // easier to initialize with string literal (
  char family_name[] = "Glenn";

  printf("%s\n", given_name);

  // allocate enough space for the family name, punctiation, given name, and \0
  char full_name[strlen(family_name) + 2 + strlen(given_name) + 1];
  strcpy(full_name, family_name);
  strcat(full_name, ", ");
  strcat(full_name, given_name);

  printf("%s\n", full_name);

  // pass strings to functions like any other array
  printf("%d\n", count_spaces(full_name));

  // this is very slow -- think about why make_copies is so slow
  // and how it could be improved
  char wall[4000001];
  make_copies(wall, "#", 4000000);
  printf("%s\n", wall);
}

int count_spaces(const char s[])
{
  int i = 0;
  int count = 0;
  while (s[i] != '\0')
    {
      if (isspace(s[i]))
	{
	  count++;
	}
      i++;
    }
  return count;
}

void make_copies(char dest[], char src[], int n)
{
  strcpy(dest, "");
  for (int i = 0; i < n; i++)
    {
      strcat(dest, src);
    }
}
