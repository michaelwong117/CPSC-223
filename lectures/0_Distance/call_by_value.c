#include <stdio.h>

int increment(int x);

int main(){
	int a  = 10;
	increment(a);

	printf("a=%d\n", a);

}

int increment(int x){
	x = x + 1;
}