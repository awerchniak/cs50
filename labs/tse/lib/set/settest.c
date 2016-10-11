/* 
 * bagtest.c - test program for bag data structure
 *	Trying to cause memory leaks, but bag is strong!!
 * Andy Werchniak, July 2016
 */
 
/* Libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

/* universal constants*/
#define MAX_ITER 100
#define STRLEN 15

/* Structure for testing*/
typedef struct test{
	char *string;
} test_t;

/* User defined functions */
void delete_test(void *data);
void delete_int(void *data);
void rand_string(int length, char* str);

/* Driver */
int main(void)
{
	int i;
	int *j;
	char *str;
	test_t *new;
	
	//create two sets
	set_t *set1 = set_new(delete_test, NULL);				//no need to print the sets
	set_t *set2 = set_new(delete_int, NULL);
	
	//fill the first set
	printf("Filling set 1\n");
	for(i=0;i<MAX_ITER;i++){
		str = malloc(sizeof(char) * (STRLEN + 1));
		if(str != NULL){									//make sure malloc doesn't fail
			rand_string(STRLEN, str);
			new = malloc(sizeof(test_t *));
			if(new != NULL){								//check this malloc
				new->string = str;
				set_insert(set1,str,new);
			} else{
				fprintf(stderr, "Error with malloc.\n");
				free(str);
			}
		} else fprintf(stderr, "Error with malloc.\n");
		//print every 10th set to test find function
		if(!(i%10))
			printf("Found string '%s' in set1\n", (char *)((test_t *)set_find(set1,str))->string);
	}
	
	//fill the second set
	printf("Filling set 2\n");
	for(i=0;i<MAX_ITER;i++){
		str = malloc(sizeof(char) * (STRLEN + 1));
		if(str != NULL){									//make sure malloc doesn't fail
			rand_string(STRLEN, str);
			j = malloc(sizeof(int));
			if(j!= NULL){									//check this malloc
				*j = i;
		
				set_insert(set2,str,j);
		
				//print every 10th set to test find function
				if(!(i%10))
					printf("Found int '%d' in set2\n", *(int *)set_find(set2,str));
				free(str);
			} else{
				fprintf(stderr, "Error with malloc.\n");
				free(str);
			}
		} else fprintf(stderr, "Error with malloc.\n");
	}
	
	//delete the two sets
	set_delete(set1);
	set_delete(set2);
	
	return(0);
}

/*	User-defined delete functions for testing	*/
void delete_test(void *data){
	free(((test_t *)data)->string);
	free(data);
}

void delete_int(void *data){
	free(data);
}

/*Function to generate random strings for testing
 *	Written by Douglas Tallmadge and distributed to CS50 section
 *	February 2016 - July 2016
 */
void rand_string(int length, char* str){

	char* charSet = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < length; i++)
	{

		int r = rand() % 26;

		str[i] = charSet[r];
	}
}
