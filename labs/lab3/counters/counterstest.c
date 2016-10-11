/* 
 * counterstest.c - test program for simple counters
 *
 * Andy Werchniak, July 2016
 */

/* Libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "counters.h"

/* Universal constants */
#define ITER_ONE 467
#define ITER_TWO 5647
#define ONE_CTRS 10
#define TWO_CTRS 13

/* Driver*/
int main(void)
{
 	int i;
 	
 	//make two counters
 	counters_t *first = counters_new();
 	counters_t *second = counters_new();
 	
 	//fill the first counter
 	printf("Filling first counters\n");
 	for(i=0;i<ITER_ONE;i++){
 		counters_add(first, i%ONE_CTRS);
 	}
 	
 	//read out the counts in there
 	printf("Counter 1's counts:\n");
 	for(i=0;i<ONE_CTRS;i++){
 		printf("%d: %d\n", i, counters_get(first,i));
 	}
 	printf("\n");
 	
 	//fill the second counter
 	printf("Filling second counters");
 	for(i=0;i<ITER_TWO;i++){
 		counters_add(first, i%TWO_CTRS);
 	}
 	
 	//read out the counts in there
 	printf("Counter 2's counts:\n");
 	for(i=0;i<TWO_CTRS;i++){
 		printf("%d: %d\n", i, counters_get(first,i));
 	}
 	printf("\n");
 	
 	//delete both of the counters
 	counters_delete(first);
 	counters_delete(second);
	return(0);
}