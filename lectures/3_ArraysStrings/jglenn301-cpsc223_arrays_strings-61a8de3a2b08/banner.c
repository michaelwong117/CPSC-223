#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void make_banner(char dest[], char src[], int n);
void print_banner_nm(size_t n, const char *part);
void print_banner(size_t n, const char *part);


int main()
{
	int copies = 0;
	printf("How many copies: ");
	scanf("%d", &copies);
	char piece[10000];
	printf("Piece pattern: ");
	scanf("%s", piece); // %s scans the whole string in
	// %d scans one integer in
	// scanf("%d", piece);

	print_banner(copies, piece);

	// Debug check to see what arr piece contains

	// for (int i = 0; i < copies; i++)
	// {
	// 	printf("%c", piece[i]);
	// }

	// Manual writing

	// char piece[] = "==++++==";
	// int copies = 0;
	// printf("How many copies: ");
	// scanf("%d", &copies);
	// char banner[strlen(piece) * copies + 1];
	// strcpy(banner, "");

	// for (int i = 0; i < copies; i ++)
	// {
	// 	strcat(banner, piece);
	// }
	// printf("%s\n", banner);

	return 0;


	// If you enter 10^6 copies
	// It will take a long time

	// Core problem: in order for strcat to add something to the end of a string
	// strcat has to go through the entire sing
}
void make_banner(char dest[], char src[], int n)
{
	strcpy(dest, "");
	int len = strlen(src);
	for (int i = 0; i < n; i++) //iterates n times
	{
		strcat(dest, src);
	}
}				

// void strcat(char dest[], char src[])
// {
// 	int i = 0;
// 	while (dest[i] != '\0') //for n-char dest, n+1 executions
// 	{
// 		i++;
// 	}
// 	int j = 0;
// 	while (src[j] != '\0') { dest[i] = src[j]; i++; j++; }
// 	dest[i] = '\0';

// }

void print_banner_nm(size_t n, const char *part)
{
	char banner[strlen(part) * n + 1];
	for (size_t copy = 0; copy < n; copy++)
	{
		for(size_t i = 0; i < strlen(part); i++) // n chars in part
		{										 // strlen total O(n * m)
			banner[copy * strlen(part) + i] = part[i];
		}
	}

	banner[strlen(part) * n] = '\0';
	printf("%s\n", banner); 

}

void print_banner(size_t n, const char *part) 
{
	size_t len = strlen(part);
	char banner[len * n + 1];

	for (size_t copy = 0; copy < n; copy++)
	{
		for(size_t i = 0; i < len; i++) // n chars in part
		{										 
			banner[copy * len + i] = part[i];
		}
	}

	banner[len * n] = '\0';
	printf("%s\n", banner); 

}






