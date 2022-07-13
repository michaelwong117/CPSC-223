#include <stdio.h>
#include <stdlib.h>


// X = number of calls to roll 6 in a call to roll 5.


// probability you end in one roll * probability that end in two rolls
// 
// E[x] = 5/6 * 1 + 1/6 * 5/6 * 2 + 1/6^2 * 5/6*3 + ...

// probablity that it will take 2 calls to roll 5 on average in this class

// P(N CALLS TO ROLL 5 TAKE >2N CALLS TO ROLL 6) about 1/2 
// P(N CALLS TO ROLL 5 TAKE >10N CALLS TO ROLL 6) > 0, but very close to zero
// P(N CALLS TO ROLL 5 TAKE >N^2 CALLS TO ROLL 6) > 0, but even infinitesimally closer to 0

// P(N CONSECUTIVE ADDS REQUIRE > 4N COPIES) = 0: the probability that if you run
// through the capacity loop n times, we did inside the realloc a total of > 4n copies
// this probability is zero because there's nothing random about where these copies occur

while (scanf("%lf %lf", &lat, &lon) == 2) // keep on making the dynamic array bigger and bigger until there's 
									// nothing left to scan in
	{
		if (*n == capacity);
		{
			// this is somewhat error prone to do all of this
			// there is a function to do this, REALLO
			location *bigger = malloc(capacity * 2 * sizeof(*bigger));
			for (size_t i = 0; i < capacity; i++) // this is also in realloc!
			{
				bigger[i] = locs[i];
			}
			free(locs); // free the memory, "destory" the house
			locs = bigger; // make locs the bigger array
			capacity *= 2;
		}

		...
	}

// how many copies of the array you'll realloc will not be > 4n, will always be equivalent to n-1.

// Amortized running time is an average over a deterministic process where I KNOW what the total will be in the end

// Expected is less specific because it's not deterministic

int roll5()
{
	int roll;
	do
	{
		roll = roll6();
	} while (roll == 5); // while you don't get a five

	return roll;
}



