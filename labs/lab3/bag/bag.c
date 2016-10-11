/*
 *	bag.c - like a set, but unorganized (may have repeats)
 *		Implemented as a singly linked list
 *
 *	Andy Werchniak, July 2016
 */

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include "bag.h"

/**************** global variables ****************/
														
/**************** local types ****************/
typedef struct item {
	void *data;
	struct item *next;
} item_t;

/**************** global types ****************/
typedef struct bag {
	item_t *head;
	void (*item_delete)(void *data);
	void (*print_item)(void *data);
} bag_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */


/**************** bag_new ****************/
bag_t *bag_new(void (*item_delete)(void *data), void (*print_item)(void *data))
{
	bag_t *bag = (bag_t *)malloc(sizeof(bag_t));		//allocate memory for your bag
	
	if(bag == NULL)										//in case malloc failed
		return NULL;
	else{
		bag->head = NULL;								//head is null
		bag->item_delete = item_delete;					//link to user specified functions
		bag->print_item = print_item;
		return bag;										//return pointer to new bag
	}
}


/**************** bag_insert ****************/
void bag_insert(bag_t *bag, void *data)
{
	item_t *new;
	
	if (bag == NULL){									//check if bag_new() has been called
		fprintf(stderr, "Error! Cannot insert to an empty bag!\n");
	} else if (data == NULL){							//make sure data has been specified
		fprintf(stderr, "Error! Cannot insert element with NULL pointer\n");
	} else{												//otherwise, insert!
		new = (item_t *)malloc(sizeof(item_t));			//allocate space for new item
		new->data = data;								//link data
		if(bag->head == NULL)							//check if it's the first item
			new->next = NULL;							//if so, there's no next
		else
			new->next = bag->head;						//otherwise, next is head
		bag->head = new;								//set head to the new item
	}
}

/**************** bag_extract ****************/
void *bag_extract(bag_t *bag){
	item_t *temp;
	void *data;
	
	if(bag == NULL || bag->head == NULL){				//check if bag is empty
		fprintf(stderr, "Error! Cannot extract from empty bag!\n");
		return NULL;									//return NULL if there's a problem
	} else{
		temp = bag->head;								//otherwise, go to first item
		data = temp->data;								//get the data from it
		
		if(bag->head->next != NULL)						//if we're not at the end, just skip item
			bag->head = bag->head->next;
		else
			bag->head = NULL;							//if we are, set head to null
		
		free(temp);										//free the item we just extrated
		return(data);									//return its data
	}
}

/**************** bag_delete ****************/
void bag_delete(bag_t *bag)
{
	item_t *temp, *ptr = bag->head;						//start at the beginning of the bag
	
	if(bag->item_delete == NULL){						//check if delete function is specified
		fprintf(stderr, "Error! No item_delete function specified for bag structure.\n");
		return;
	}
	
	//start at head and iterate through items until the end, deleting each one
	while(ptr != NULL){
		temp = ptr->next;								//store address of next item
		if(ptr->data != NULL)
			(*bag->item_delete)(ptr->data);				//call user's function to delete data
		if(ptr != NULL){
			free(ptr);									//free the item in the list
			ptr = NULL;
		}
		ptr = temp;										//move onto next item
	}
	
	if(bag != NULL){
		free(bag);										//free the whole bag
		bag = NULL;
	}
	
	printf("Bag successfully deleted.\n");
}

/**************** print_bag ****************/
void print_bag(bag_t *bag)
{
	item_t *ptr = bag->head;							//start at the beginning
	
	if(bag->print_item == NULL){						//make sure print function is specified
		fprintf(stderr, "Error! No print_item function specified for bag structure.\n");
		return;
	}
	
	if(ptr == NULL)										
		printf("Empty bag!\n");							//if the bag is empty
		
	while(ptr != NULL){
		(*bag->print_item)(ptr->data);					//otherwise call user's print
		ptr = ptr->next;								// function on each item
	}
}
