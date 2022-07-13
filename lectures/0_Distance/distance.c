#include <stdio.h>
#include <math.h>


/**
*gcc -o Distance -wall -std=c99 -pedantic -g distance.c -lm ./Distance
*/

#ifndef __MORE_MATH_H__ //if this thing is not defined, define it
#define __MORE_MATH_H__ 


// Returns the colatitude of the given latitude. 
// The colatitude is the degrees form the north pole to the given latitude.

double colatitude(double lat){
 // Returns given angle converted from degrees to radians
 // @param a an angle measured in degrees
 // @return the angle in radians
}
double to_radians(double a){
 // @return the angle in radians
}


#endif



int main(){

	double prev_lat, prev_lon;
	double curr_lat, prev_lon;
	
	scanf("%lf %lf", &prev_lat, &prev_lon);
	scanf("%lf %lf", &curr_lat, &curr_lon);

	double prev_lat = 41.2679, prev_lon = -72.8869;
	double curr_lat = 39.1774, curr_lon = -76.6684;

	// compute colatitudes and convert to radians
	double colat1 = colatitude(prev_lat);
	colat1 = to_radians(colat1);
	double colat2 = colatitude(curr_lat);
	colat2 = to_radians(colat2);

	// compute difference in longitudes in raidans 



	return 0;
}


double colatitude(double lat){
 // Returns given angle converted from degrees to radians
 // @param a an angle measured in degrees
 // @return the angle in radians
	return 90 - lat;
}
double to_radians(double a){
	return (3.14159265358973284 / 180.0) * a;
}






