/* 
 * bagtest.c - test program for bag data structure
 *	Trying to cause memory leaks, but bag is strong!!
 * Andy Werchniak, July 2016
 */
 
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

/* Universal constants*/
#define MAX_ITER 100000
#define STRLEN 11

/* Structure for testing*/
typedef struct test{
	char *string;
} test_t;

/* User defined functions */
void delete_test(void *data);
void delete_int(void *data);

/* Driver */
int main(void)
{
	int i;
	int *j;
	char *str;
	test_t *new;
	
	//create two bags
	bag_t *bag1 = bag_new(delete_test, NULL);				//no need to print the bags
	bag_t *bag2 = bag_new(delete_int, NULL);
	
	//fill the first one
	printf("Filling bag 1\n");
	for(i=0; i<MAX_ITER; i++){
		str = malloc(sizeof(char) * (STRLEN + 1));
		strcpy(str, "Test String");
		
		if(str != NULL){
			new = malloc(sizeof(test_t *));
		
			new->string = str;
		
			bag_insert(bag1, new);
		} else fprintf(stderr,"Malloc failed on iteration %d\n", i);
	}		
	
	//take some out of the first back to check if it's ok
	printf("Extract a few from the bag:\n");
	for(i=0; i<MAX_ITER/10000;i++){
		new = ((test_t *)bag_extract(bag1));
		str = new->string;
		printf("%s\n", str);
		free(str);
		free(new);
	}
	
	//fill the second bag
	printf("Filling bag 2\n");
	for(i=0; i<MAX_ITER; i++){
		j = malloc(sizeof(int));
		*j = i;
		bag_insert(bag2, j);
	}
	
	//take some out of the second bag to check if it all worked
	printf("Extract a few from the bag:\n");
	for(i=0; i<MAX_ITER/10000;i++){
		j = (int *)bag_extract(bag2);
		printf("%d\n", *j);
		free(j);
	}
	
	//delete the bags
	bag_delete(bag1);
	bag_delete(bag2);
	
	return(0);
}

/*	User-defined delete functions for testing. 
 */
void delete_test(void *data){
	free(((test_t *)data)->string);
	free(data);
}

void delete_int(void *data){
	free(data);
}
