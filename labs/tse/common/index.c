/*
 * index - a set of functions for creating, saving, loading the index
 *
 * See index.h for interface descriptions.
 *
 * Andy Werchniak, Summer 2016
 */
 
/**************** libraries ****************/
#include "index.h"								//linked library
#include <ctype.h>								//isdigit()
#include <stdlib.h>								//free()
#include <stdio.h>								//fprintf(), etc
#include <stdbool.h>							//bool data type
#include <unistd.h>								//acces()
#include <string.h>								//strlen
#include "hashtable/hashtable.h"				//hashtable functions
#include "counters/counters.h"					//counters_t, etc.
#include "file.h"								//readline()

/**************** local functions ****************/
/* not visible outside this file */
/* 	See function definitions for descriptions */
void index_delete_helper(void *data);
void index_print_helper(FILE *fp, char *key, void *data);
int read_ints(char *buffer, int *pos, int *first, int *second);
void reverse_string(char *str);

/**************** global functions ****************/
/* that is, visible outside this file */

 /*index_new
 *	create a new index data structure, implemented as hashtable of counters
 *	arguments: number of slots in the table
 *	returns a pointer to the new table
 */
index_t *index_new(const int num_slots)
{
	return((index_t *)hashtable_new(num_slots, index_print_helper, index_delete_helper));
}

/*index_insert
 *	insert an element into the index
 *	arguments: index to insert into, key/data pair to insert
 *	return: true if successful, false if unsuccessful.
 */
bool index_insert(index_t *ht, char *key, counters_t *occurrences)
{
	return(hashtable_insert((hashtable_t *)ht,key,occurrences));
}

/*index_find
 *	Find the counters with a given key
 *	arguments: index to look in, key to look for
 *	return: the data if found, or null if not
 */
counters_t *index_find(index_t *ht, char *key)
{
	return((counters_t *)hashtable_find((hashtable_t *)ht,key));
}

/*index_load
 *	Load the contents of an external index file into an index_t data structure
 *	arguments: index to load into, name of file containing contents
 *	Warning: this will allocate space for each counters_t in the index. User
 *		is responsible for freeing.
 *	Limitations: only functional for keys up to 1000 characters long.
 */
void index_load(index_t *index, char *fileName)
{
	//check parameters to ensure they are correct
	if(access(fileName, F_OK) == -1) {
		fprintf(stderr, "Please enter a valid fileName.\n");
		return;
    }
    
    if(index == NULL){
    	fprintf(stderr, "Must allocate space for index before filling\n");
    	return;
    }
    
    //create file pointer and open file for reading
    FILE *inFile = fopen(fileName, "r");
    
    //declare other necessary variables
    char *buffer;
    char key[1000];					//allows for a word to be up to 1000 characters long
    int pos,countersKey, countersCount;

    while((buffer = readline(inFile)) != NULL){
    	//start reading at the end of the buffer
    	pos = strlen(buffer);

    	//first string on the line gives you the key
    	sscanf(buffer, "%s", key);
    	
    	//create a new counters
    	counters_t *new = counters_new();
    	
    	//read key-integer pairs from back of the buffer to the front
    	while(read_ints(buffer, &pos, &countersKey, &countersCount) >0){
    		//fill the counters
    		counters_set(new, countersKey, countersCount);
    	}
    	//free the memory
    	free(buffer);
    	
    	//insert the key,counters pair into the index
    	index_insert(index, key, new);
    }
	
	//close file
	fclose(inFile);
}

/*index_save
 *	Save the contents of an index data structure to an external file
 *	arguments: index to save, name of file to save to
 */
void index_save(index_t *index, char *fileName)
{
	//verify parameters
	if(index == NULL || fileName == NULL)
		return;
	
	//create file pointer and open file for writing
	FILE *fp = fopen(fileName, "w");

	//let hashtable_print do the work!
	hashtable_print(fp, (hashtable_t *)index);
	
	//close the file
	fclose(fp);
	
}

/*index_delete
 *	delete an entire index data structure
 *	arguments: index to delete
 */
void index_delete(index_t *ht)
{
	hashtable_delete((hashtable_t *)ht);
}

/*index_delete_helper
 *	data in the set just a counters, so just call counters_delete!
 */
void index_delete_helper(void *data)
{
	counters_delete((counters_t *)data);
}

/*index_print_helper
 *	tells hashtable_print how to tell set_print how to print 
 *		the data in it
 *	since in index_t, the sets are simply key, counters pairs
 *		all we do is print the key and cal counters_print
 *
 */
void index_print_helper(FILE *fp, char *key, void *data)
{	
	//print the key and then let counters_print do the work!
	fprintf(fp, "%s ", key);
	counters_print(fp, (counters_t *)data);
	fprintf(fp, "\n");
}

/*read_ints
 *	Andy Werchniak, August 2016
 *
 *	Reads couples of two integers from a character buffer, starting at the
 *		end and moving to the front. Updates the two integers passed by reference
 *		as well as the position in the buffer to read. Finishes when it reaches
 *		buffer[0].
 *	arguments: buffer to read from, position to start, numbers to return
 *	returns: -1 if unsuccessful, #digits read if successful (2)
 *
 *	ASSUMPTIONS:
 *	1. All numbers encountered are positive (will ignore negative signs)
 *	2. Numbers appear in pairs (will couple into pairs, and ignore first number
 *		if the string contains an odd number of integers)
 *	3. No floating point values- all numbers will be interpreted as integers
 *		(a decimal point will be interpreted as a space between separate integers)
 *	NON-ASSUMPTIONS:
 *	1. User may sprinkle any characters in between the numbers. read_ints
 *		will simply skip over the intermediaries and scan the digits
 *	LIMITATIONS:
 *	1. read_ints will only work with integers up to 100 digits long.
 *		else, it will truncate using the last 100 digits
 */
int read_ints(char *buffer, int *pos, int *first, int *second)
{
	//declare relevant variables
	char c;
	int i,count = 0;
	char int_string[100];			//integers can be up to 100 digits long
	
	while((*pos)>=0 && count<2){
		//set c to the character at the current position
		c=buffer[*pos];
		
		if(isdigit(c)){
			//if it's a digit, we have to keep scanning until we find something that isn't
			//or we reach the beginning of the file or the max capacity of int_string
			for(i = 0; i<100 && (*pos)>=0 && isdigit(c=buffer[*pos]); i++){
				int_string[i] = c;	//copy c to the correct position in int_string
				(*pos)--;			//decrement the position
			}
			//null terminate
			int_string[i] = '\0';
			
			//reverse its order, in case it's multiple digits
			reverse_string(int_string);

			//set the parameters passed by value
			if(count == 0){
				*second = atoi(int_string);		//convert to int
				count++;
			} else if(count == 1){
				*first = atoi(int_string);		//convert to int
				count++;
			} else{
				fprintf(stderr, "Something went wrong.\n");
				return(-1);
			}
		}
		//decrement the position
		(*pos)--;
	}
	
	//return 2 if successful, -1 if unsuccessful
	if(count != 2)
		return(-1);
	else return(count);
}

/*reverse_string
 *	program to reverse the order of a string
 *	Written to help with read_ints (must read in reverse order because
 *		counters_insert and counters_print used together create a reversal
 *		of order)
 */
void reverse_string(char *str)
{
	//get the length of the input string			
	int len = (int)strlen(str);
	
	//allocate space for a temporary string and copy the contents to it
	char *temp = malloc(len+1);
	strcpy(temp, str);
	
	//copy temp to the original string, reversing the order as we go
	for(int i=len-1; i>=0; i--){
		str[(len-1)-i] = temp[i];
	}
	
	//always remember to free!
	free(temp);
}