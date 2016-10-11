/*
 *	set.c - set of (string,data) pairs
 *		Implemented as linked list with a null sentinel
 *
 *	Andy Werchniak, July 2016
 */
 
 /* Libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**************** global variables ****************/

/**************** local types ****************/
typedef struct entry {
	char *key;
	void *data;
	struct entry *next;
} entry_t;

/**************** global types ****************/
typedef struct set{
	entry_t *head;
	void (*print_item)(FILE *fp, char *key, void *data);
	void (*item_delete)(void *data);
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/*	Create a new empty set
 *	User passes in functions for deleting their desired data type
 * 		as well as printing their data type
 */
set_t *set_new(void (*item_delete)(void *data), void (*print_item)(FILE *fp, char *key, void *data))
{
	set_t *set = (set_t *)malloc(sizeof(set_t));		//malloc space for the set
	
	if(set == NULL)										//in case malloc failed
		return NULL;
	else{
		set->head = NULL;								//if not, set head to null
		set->item_delete = item_delete;					//	link user's delete function
		set->print_item = print_item;					//	link user's print function
		return set;										//	return pointer to new set
	}
}

//find data for given key or return null
void *set_find(set_t *set, char *key)
{
	entry_t *ptr;
	
	if(set == NULL || set->head == NULL){				//make sure set isn't empty
		//fprintf(stderr, "Error! Cannot find in empty set.\n");
		return(NULL);
	}
	
	for(ptr = set->head; ptr != NULL; ptr = ptr->next){	//iterate through set
		if(ptr->key != NULL && !strcmp(key,ptr->key))	//if found, return data
			return(ptr->data);
	}
	
	return(NULL);										//if not, return null
}

//return false if key already exists, and true if new item was inserted.
//If set is NULL, or you encounter other error, return false.
//copy key string
bool set_insert(set_t *set, char *key, void *data)
{
	entry_t *new, *ptr;
	
	if (set == NULL){									//check if set exists
		fprintf(stderr, "Error! Cannot insert until set is malloc'd.\n");
		return(false);									//	return false if not
	} else if (key == NULL){
		fprintf(stderr, "Error! Cannot link NULL key.\n");
		return(false);									//	return false if not
	} else{ 											//otherwise, check if key is already
														//	in the set
		for(ptr = set->head; ptr != NULL; ptr = ptr->next){
			if (ptr->key != NULL && !strcmp(key,ptr->key)){
				//fprintf(stderr, "Key '%s' is already in the set!\n", key);
				return(false);							//if found, notify user and return
			}											//	false
		}
		
														//if not found, insert it at the end
		new = (entry_t *)malloc(sizeof(entry_t));		//malloc space
		if(new == NULL){								//check that malloc was successful
			fprintf(stderr, "Problem mallocing for entry.\n");
			return false;								//	if not, return false
		}
		
		new->key = malloc(strlen(key)+1);				//malloc space for key string
		if(new->key != NULL){							//if malloc is successful
			new->data = data;							//link data
			strcpy(new->key,key);						//copy key string
			if(set->head == NULL)						//if it's an empty set, no next
				new->next = NULL;
			else
				new->next = set->head;					//otherwise previous head is next
			set->head = new;							//latest item becomes the head
			return(true);								//return success
		}else{											//if there was an error with malloc
      		fprintf(stderr, "Problem mallocing for key.\n");
      		free(new);									//free the new node
      		return false;								//return failure
		}
	}
}

/* Delete the set
 *	Note: user must define item_delete function and pass it
 *		to set_new for this to work
 */
void set_delete(set_t *set)
{
	entry_t *temp, *ptr = set->head;					//start at beginning of list
	
	//make sure the user specified an item_delete function
	if(set->item_delete == NULL){
		fprintf(stderr, "Error! No item_delete function specified for set structure.\n");
		//break if not
		return;	
	}

	//start at head and iterate through items until the end, deleting each one
	while(ptr != NULL){
		temp = ptr->next;								//store next address to move forward
		if(ptr->key != NULL){
			free(ptr->key);								//free memory from key string
			ptr->key = NULL;
		}
		if(ptr->data != NULL){
			(*set->item_delete)(ptr->data);				//user's function deletes data
		}
		if(ptr != NULL){
			free(ptr);									//free the entry in the set
			ptr = NULL;
		}
		ptr = temp;										//move onto next element
	}
	
	if(set != NULL){
		free(set);										//free the entire data structure
		set = NULL;
	}
	
	//printf("Set successfully deleted.\n");
}

/*	Print set to input file pointer
 *
 */
void print_set(FILE *fp, set_t *set)
{
	entry_t *ptr = set->head;
	
	//make sure the user specified a print function
	if(set->print_item == NULL){
		fprintf(stderr, "Error! No print_item function specified for set structure.\n");
		return;
	}
	
	if(ptr == NULL)											//if the set is empty
		printf("Empty set!\n");
		
	while(ptr != NULL){										//iterate through set and use user's
		(*set->print_item)(fp, ptr->key, ptr->data);		//	function to print each element
		ptr = ptr->next;
	}
}

/* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void set_iterate(set_t *set, void (*itemfunc)(void *arg, const char *key, void *data), void *arg)
{
	if(set == NULL){
		fprintf(stdout, "Error! Cannot iterate through empty set.\n");
		return;
	} else if(itemfunc == NULL){
		fprintf(stdout, "Error! Must define item function.\n");
		return;
	} else{
		for(entry_t *node = set->head; node!= NULL; node=node->next)
			(*itemfunc)(arg,node->key,node->data);
	}
}