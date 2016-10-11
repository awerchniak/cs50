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
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
 
/**************** counters_new ****************/
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
 
/**************** counters_delete ****************/
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
	
	printf("Counters successfully deleted\n");
}