/* hashtabletest.c - test function for hashtable.c class
 *
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "jhash.h" 
#include "../set/set.h"

/* Universal constants */
#define h_one_size 4
#define h_two_size 12
#define MAX_ITER 100
#define STRLEN 15

/* Data type for testing*/
typedef struct test{
	char *string;
} test_t;

/* User defined functions */
void delete_int(void *data);
void delete_test(void *data);
void rand_string(int length, char* str);

/* Driver */
int main(void)
{
	int i;
	int *j;
	char *str;
	test_t *new;
	
	//create two tables
	hashtable_t *ht1 = hashtable_new(h_one_size, NULL, delete_test);
	hashtable_t *ht2 = hashtable_new(h_two_size, NULL, delete_int);
	
	//fill the first table with strings
	printf("Filling set 1\n");
	for(i=0;i<MAX_ITER;i++){
		str = malloc(sizeof(char) * (STRLEN + 1));
		if(str != NULL){
			rand_string(STRLEN, str);
			new = malloc(sizeof(test_t *));
			if(new != NULL){
				new->string = str;
		
				//insert into the table
				hashtable_insert(ht1,str,new);
		
				//every tenth time test find and print
				if(!(i%10))
					printf("Found string '%s' in ht1\n", (char *)((test_t *)hashtable_find(ht1,str))->string);
			} else{
				fprintf(stderr, "Error with malloc.\n");
				free(str);
			}
		} else fprintf(stderr, "Error with malloc.\n");
	}
	
	//fill the second table with integers
	printf("Filling set 2\n");
	for(i=0;i<MAX_ITER;i++){
		str = malloc(sizeof(char) * (STRLEN + 1));
		if(str != NULL){
			rand_string(STRLEN, str);
			j = malloc(sizeof(int));
			if(j != NULL){
				*j = i;
		
				//insert into the table
				hashtable_insert(ht2,str,j);
			
				//every tenth time test find and print
				if(!(i%10))
					printf("Found int '%d' in ht2\n", *(int *)hashtable_find(ht2,str));
				free(str);
			} else{
				fprintf(stderr, "Error with malloc.\n");
				free(str);
			}
		} else fprintf(stderr, "Error with malloc.\n");
	}
	
	//delete the tables
	hashtable_delete(ht1);
	hashtable_delete(ht2);
	
	return(0);
}

//only have to free the integer passed in
void delete_int(void *data){
	free(data);
}

//have to free the string and the whole structure
void delete_test(void *data)
{
	free(((test_t *)data)->string);
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