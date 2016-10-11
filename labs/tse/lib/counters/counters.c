/*
 *	counters.c - linked list of counters, with int key and int count counter
 *		Implemented as linked list with a null sentinel
 *
 *	Andy Werchniak, July 2016
 */

/* Libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "counters.h"
 
/**************** global variables ****************/
														
/**************** local types ****************/
typedef struct counter{
 	int *key;
 	int count;
 	struct counter *next;
} counter_t;
 
/**************** global types ****************/
typedef struct counters{
	counter_t *head;
} counters_t;
 
/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
 
/**************** counters_new ****************/
/*	Create a new counters_t data structure and allocate memory for it
 *	The user is responsible for freeing memory that counters_new allocates
 */
counters_t *counters_new(void)
{
	//malloc space for counters
 	counters_t *new = (counters_t *)malloc(sizeof(counters_t));
 	
 	
 	if(new == NULL){									//check if malloc failed
 		fprintf(stderr, "Error with malloc! No counters created.\n");
 		return(NULL);
 	}else{												//otherwise, set head to NULL
 		new->head = NULL;								//and return pointer to counters
 		return(new);
 	}
}

/**************** counters_add ****************/
/* If a key exists, increment its value. If not, create
 *	a new key and set its value to 1. Again, the user is
 *	responsible for freeing all memory.
 */
void counters_add(counters_t *ctrs, int key)
{
 	counter_t *ptr;
 	
 	if(key<0){											//make sure key is positive
 		fprintf(stderr,"Error! Keys must be positive integers.\n");
 		return;											//if not, break
 	}
 	
 	if (ctrs == NULL){									//check if ctrs is NULL first!!!!
 		fprintf(stderr,"Error! Cannot add to a null counters.\n");
 		return;
 	} else{												//otherwise try to find the key
 		for(ptr=ctrs->head; ptr!=NULL;ptr=ptr->next){
 			if(*(ptr->key) == key){						//if we find it, increment
 				(ptr->count)++;
 				return;									//and break
 			}
 		}
 	}
  														//if we never found it, create a new one
 	ptr = (counter_t *)malloc(sizeof(counter_t));		//malloc new counter
 	if(ptr != NULL){									//check if malloc failed
 		ptr->key = malloc(sizeof(int));					//malloc new key
 		if(ptr->key != NULL){							//check if malloc failed
 			*(ptr->key) = key;							//set new key value
 			ptr->count = 1;								//set count to 1
 			if(ctrs->head == NULL)						//check if it's the first
 				ptr->next = NULL;						//if so, nothing next
 			else
 				ptr->next = ctrs->head;					//otherwise, add to the list
 			ctrs->head = ptr;							//set head to new item
 		} else{
 			fprintf(stderr, "Error with malloc! No counter added.\n");
 			free(ptr);
 		}
 	} else {
 		fprintf(stderr, "Error with malloc! No counter added.\n");
 	}
 	
}
 
/**************** counters_get ****************/
/* return the counters structure with a given key
 * 	in the given counters_t.
 *	return 0 if it does not exist
 */
int counters_get(counters_t *ctrs, int key)
{
 	counter_t *ptr;
 	
 	if(ctrs != NULL)									//check if ctrs is NULL
 		ptr = ctrs->head;								//if not, we will start at the head
 	else{												//otherwise, exit
 		fprintf(stderr, "Cannot get key from empty counters!\n");
 		return(0);
 	}
 	
 	while (ptr!= NULL){									//iterate through counters until
 		if(*(ptr->key) == key)								//	found, and return its count
 			return(ptr->count);
 		else ptr=ptr->next;
 	}
 	
 	return(0);											//0 means no counter found
}
 
/**************** counters_print() ****************/
/* print the entire counters data structure       */
void counters_print(FILE *fp, counters_t *ctrs)
{
	if (ctrs == NULL) {
		fputs("(null)", fp);
		return; // bad counters
	} else {
		// scan the counters
		for (counter_t *node = ctrs->head; node != NULL; node = node->next) {
			// print the current node
			fprintf(fp, "%d %d ", *(node->key), node->count);
		}
	}
	return;
} 

/**************** counters_delete ****************/
/* delete the entire counters_t data structure   */
void counters_delete(counters_t *ctrs)
{
 	counter_t *temp, *ptr;								
 	ptr = ctrs->head;									//start at the first counter
 	
	while(ptr != NULL){									//go until end of list
		temp=ptr->next;									//temp pointer so we can move on
		free(ptr->key);
		free(ptr);										//free the data in that spot
		ptr = temp;										//move onto next one
	}
	
	ctrs->head=NULL;
	free(ctrs);											//free the entire data structure
	ctrs=NULL;
	
	//printf("Counters successfully deleted\n");
}

/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 * User is responsible for freeing memory from new counter_t
 */
void counters_set(counters_t *ctrs, int key, int count)
{
	counter_t *ptr;
	
	if(ctrs==NULL){
		fprintf(stderr,"Error! Cannot set empty counters.\n");
		return;
	} else if(counters_get(ctrs, key) == 0){			//if counter does not exist
		ptr = (counter_t *)malloc(sizeof(counter_t));	//malloc new counter
 		if(ptr != NULL){								//check if malloc failed
 			ptr->key = malloc(sizeof(int));				//malloc new key
 			if(ptr->key != NULL){						//check if malloc failed
 				*(ptr->key) = key;						//set new key value
 				ptr->count = count;						//set count to input value
 				if(ctrs->head == NULL)					//check if it's the first
 					ptr->next = NULL;					//if so, nothing next
 				else
 					ptr->next = ctrs->head;				//otherwise, add to the list
 				ctrs->head = ptr;						//set head to new item
 			} else{
 				fprintf(stderr, "Error with malloc! No counter added.\n");
 				free(ptr);
 			}
 		} else {
 			fprintf(stderr, "Error with malloc! No counter added.\n");
 		}
	} else{
		for(ptr=ctrs->head; ptr!=NULL;ptr=ptr->next){
 			if(*(ptr->key) == key){						//if we find it, increment
 				ptr->count = count;
 				return;									//and break
 			}
 		}
	}
}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs, void (*itemfunc)(void *arg, const int key, int count), void *arg)
{
	if(ctrs == NULL){
		fprintf(stderr, "Error! Cannot iterate empty counters.\n");
		return;
	} else if (itemfunc == NULL){
		fprintf(stderr, "Error! Must define item function.\n");
		return;
	} else{
		for(counter_t *node = ctrs->head; node != NULL; node=node->next)
			(*itemfunc)(arg,*(node->key),node->count);
	}
}
