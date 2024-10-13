#include <stdlib.h>
#include <stdbool.h>
#include "random.h"

// got this from here: https://www.quora.com/The-rand-function-in-the-standard-C-library-returns-a-uniformly-random-number-in-0-RAND_MAX-1-Does-rand-mod-n-generate-a-number-uniformly-distributed-0-n-1
int genRandom(int n) {  

	long limit = (300 / n)*n;  
	int r;  
	while (true){ 
		r = rand(); 
		if(r < limit) break; 
	} 
	return r % n;  
} 
