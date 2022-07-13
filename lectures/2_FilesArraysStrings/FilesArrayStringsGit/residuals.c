#include <stdio.h>

/**
 * Reads numbers from standard input and outputs the difference between
 * them and their mean.
 */

/**
 * Returns the mean of the numbers in the given array.
 *
 * @param n a nonnegative integer
 * @param arr an array of n doubles
 * @return the mean of the numbers in arr
 */
double calculate_mean(int n, const double arr[]);

/**
 * Modifies the given array by adding the given number to all elements.
 *
 * @param n a nonnegative integer
 * @param arr an array of n doubles
 * @param x a double
 */
void add_all(int n, double arr[], double x);

/**
 * Prints the values in the given array to standard input.
 *
 * @param n a nonnegative integer
 * @param arr an array of n doubles
 */
void print_array(int n, double arr[]);


int main()
{
  int n;

  FILE* infile;
  infile = fopen("numbers.txt", "r");

  if (!infile)
  {
    fprintf(stderr, "Could not open numbers.txt\n");
    return 1;
  }

  fscanf(infile, "%d", &n);

  int primes[] = {2, 3, 5, 7, 11};
  //Strings are just an array of char
  //End your array with the null character: \0 so that your array knows where it ends
  char name[] = {'M', 'I', 'C', 'H', 'A', 'E', 'L', '', 'W', 'O', 'N', 'G', '\0'};
  //OR
  char name[] = "MICHAEL WONG";

  int a[5] = {12, 24, 45, 99, 100};

  // scanf("%d", &n);

  double input[n];

  for (int i = 0; i < n; i++)
  {
    fscanf(infile, "%lf", &input[i]);
  }
  
  // for (int i = 0; i < n; i++) //standard input
  //   {
  //     scanf("%lf", &input[i]);
  //   }

  double mean = calculate_mean(n, input); //you have to tell a function both the array and the size of array!

  add_all(n, input, -mean);

  print_array(n, input);

  printf("\n2-D ARRAY\n");
  const int rows = 3, cols = 4;
  // 2-D array initializer -- copmiler only figures out size of 1st dimension
  double arr_2d[][4] = { {1, 2, 3, 4}, {1, 4, 9, 16}, {1, 8, 27, 64} };
  for (int r = 0; r < rows; r++)
    {
      // can pass row of 2-D array to function expecting 1-D array
      double row_mean = calculate_mean(cols, arr_2d[r]);
      add_all(cols, arr_2d[r], -row_mean);
      print_array(cols, arr_2d[r]);
    }
  
  return 0;
}

double calculate_mean(int n, const double arr[])
{
  double sum = 0;
  for(int i = 0; i < n; i++)
  {
    sum += arr[i];
  }
  return sum / n;
}


void add_all(int n, double arr[], double x)
{
    for (int i = 0; i < n; i++)
    {
      arr[i] += x;
    }
}

void print_array(int n, double arr[])
{
  printf("[");
    for (int i = 0; i < n; i++)
    {
      if (i > 0)
	{
	  printf(", ");
	}
      printf("%lf", arr[i]);
    }
    printf("]\n");
}
