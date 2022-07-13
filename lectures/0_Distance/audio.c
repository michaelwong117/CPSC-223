#include <stdio.h>
#include <math.h>

typdef state(){

}

int main(){

	//the index of the currently read sample
	int count = 0;

	//the maximuim value to count as silence
	const int threshold = 5;

	const int sample_rate = 44100;

	//the mimumum length of a gap between tracks, in seconds and samples
	const double min_gap_sec = 0.0001;
	const int min_gap_samples = (int)(sample_rate * min_gap_sec);

	const int GAP = 0;
	const int TRACK = 1;
	const int QUIET = 2;

	//the starting and ending points of the most recent track read or being read
	int start, end;
	
	int curr = 

	//the current sample value
	int value;

	while (scanf("%d", &value) > 0){
		switch (curr)
		{
			case GAP:
			if (abs(value) > threshold)
			{
				curr = TRACK;
				start = curr;
			}
			break;

			case TRACK:
			if (abs(value) <= threshold)
			{
				num_zeroes = 1;
				curr = ZEROS;
			}
			break;

			case ZEROS:
			if (abs(value) > threshold)
			{
				curr = TRACK;
				break;
			}
			else{
				
			}
			count++;
		}

	}
	return 0;
}







