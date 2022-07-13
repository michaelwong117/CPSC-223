#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// NORM is when we find an opening brace or semicolon while not inside a string, character, or comment 
enum state {NORM, STRING, CHARACTER, COMMENT, COMMENTSTAR, PARENTH};

int main()
{	
	FILE *input;
	input = stdin;
	if (input == NULL) 
	{
		perror("Error opening file"); return(-1);
	}

	unsigned char in[8000000];
	int a = 0;
	char c;

	if (input)
	{	
		while ((c = getc(input)) != EOF)
		{
			in[a] = c; a++;
		}
	}
	fclose(input);

	enum state curr = NORM;
	int lines = 0;

	for (int i = 0; i < a; i++) // loops through entire file
	{
		// You want to count { and ; as lines ONLY WHEN you're not inside of
		// " " ' ' /* */ <% %> // \n

		int j = i+1; // next char, for <% %>, // \n, /* */, 

		// printf("%c", in[i]);


		if (in[i] == '\"') // String
		{
			if (curr == NORM) // Start of a String!
			{
				curr = STRING;
				// printf(" *Start of String!* ");
			}
			else if (curr == STRING) // Already in a String, found the end
			{
				curr = NORM;
				// printf(" *End of String!* ");
			}
			else //we're inside a comment
			{
				curr = curr;
			}
		}

		if (in[i] == '\'') // Character
		{
			if (curr == NORM) // Start of a Char!
			{
				curr = CHARACTER;
				// printf(" *Start of Char!* ");
			}
			else if (curr == CHARACTER) // Already in a Char, found the end
			{
				curr = NORM;
				// printf(" *End of Char!* ");
			}
			else //we're inside a comment
			{
				curr = curr;
			}
		}

		if (in[i] == '/' && in[j] == '/') // Comment
		{
			if (curr == NORM) // Start of a Char!
			{
				curr = COMMENT;
				// printf(" *Start of Comment!* ");
			}
		}

		if (curr == COMMENT && in[i] == '\n') // Already in a Comment, found the end
		{
			curr = NORM;
			// printf(" *End of Comment!* ");

		}

		if (in[i] == '/' && in[j] == '*') /* Comment */ 
		{
			if (curr == NORM) 	// Start of a /* Comment
			{
				curr = COMMENTSTAR;
				// printf(" *Start of CommentStar!* ");

			}
		}

		if (in[i] == '*' && in[j] == '/') /* End of Comment */ 
		{
			// End of a */ Comment
			curr = NORM;
			// printf(" *End of CommentStar!* ");

		} 

		if (in[i] == '(') // Parentheses
		{
			if (curr == NORM) // Start of a /* Comment
			{
				curr = PARENTH;
				// printf(" *Start of Parentheses !* ");

			}
		}

		if (curr == PARENTH && in[i] == ')') // Parentheses
		{
			// End of Parentheses
			curr = NORM;
			// printf(" *End of Parentheses !* ");

		}

		if (curr == NORM)
		{
			if (in[i] == '<' && in[j] == '%')
			{
				lines++;
			}
			if (in[i] == '{')
			{
				lines++;
			}
			if (in[i] == ';')
			{
				lines++;
			}
		}


		// Test each set of two characters we're examining

		// printf("%c", in[i]);
		// printf("%c", in[j]);
		// printf("%c", '\n');
	}
	// printf("\n");
	printf("%d\n", lines);



	return 0;
}

