#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// NORM is when we find an opening brace or semicolon while not inside a string, character, or comment 
enum state {NORM, STRING, CHARACTER, COMMENT, COMMENTSTAR};

int main()
{	
	FILE *input;
	input = stdin;
	if (input == NULL) 
	{
		perror("Error opening file"); return(-1);
	}


	enum state curr = NORM;
	int lines = 0;

	int parenth = 0;

	char prev, c;

	prev = getc(input); //previous char

	while (prev != EOF) // loops through entire file
	{
		// You want to count { and ; as lines ONLY WHEN you're not inside of
		// " " ' ' /* */ <% %> // \n

		c = getc(input);  // next char, for <% %>, // \n, /* */, 

		if (c == EOF) break;
		if (prev == '\\' && c == '\n') continue;
		
		// printf("%c", prev);

		if (prev == '\"') // String
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

		if (prev == '\'') // Character
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

		if (prev == '/' && c == '/') // Comment
		{
			if (curr == NORM) // Start of a Comment!
			{
				curr = COMMENT;
				// printf(" *Start of Comment!* ");
			}
		}

		if (curr == COMMENT && prev == '\n') // Already in a Comment, found the end
		{
			curr = NORM;
			// printf(" *End of Comment!* ");

		}

		if (prev == '/' && c == '*') /* Comment */ 
		{
			if (curr == NORM) 	// Start of a /* Comment
			{
				curr = COMMENTSTAR;
				// printf(" *Start of CommentStar!* ");

			}
		}

		if (curr == COMMENTSTAR && prev == '*' && c == '/') /* End of Comment */ 
		{
			// End of a */ Comment
			curr = NORM;
			// printf(" *End of CommentStar!* ");

		} 

		if (prev == '(') // Parentheses
		{
			if (curr == NORM || parenth > 0) // Start of a Parentheses
			{
				parenth++;
				// printf(" *Start of Parentheses !* ");

			}
		}

		if (curr == NORM && prev == ')') // Parentheses
		{
			parenth--;
			if (parenth == 0)	
			{
				// End of Parentheses
				curr = NORM;
				// printf(" *End of Parentheses !* ");
			}

		}

		if (curr == NORM && parenth == 0)
		{
			if (prev == '<' && c == '%')
			{
				lines++;
			}
			if (prev == '{')
			{
				lines++;
			}
			if (prev == ';')
			{
				lines++;
			}
		}

		prev = c;
		// if (curr == NORM && parenth > 0)
		// {
		// 	curr = PARENTH;
		// }
		// Test each set of two characters we're examining

		// printf("%c", prev);
		// printf("%c", c);
		// printf("%c", '\n');

	}

	fclose(input);

	// printf("\n");
	printf("%d\n", lines);

	return 0;
}
