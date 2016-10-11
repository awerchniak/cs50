/*
 *	hashtable.c - hash table data structure
 *		Implemented as array of pointers to set_t structures
 *
 *	Andy Werchniak, July 2016
 */

/* Libraries */ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "jhash.h" 
#include "../set/set.h"

/**************** global variables ****************/
														
/**************** local types ****************/

/**************** global types ****************/
typedef struct hashtable {
	int slot_num;
	set_t **table;												//table is array of sets
	void (*item_delete)(void *data);
} hashtable_t;


/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
 
/**************** counters_new ****************/
hashtable_t *hashtable_new(const int num_slots, void (*item_delete)(void *data))
{
	int i,j;
	hashtable_t *ht;
	set_t *s;
	
	ht = malloc(sizeof(hashtable_t));							//malloc hashtable
	if(ht == NULL){												//check if successful
		fprintf(stderr, "Error in structure malloc!\n");
		return(NULL);											//exit if not
	}
	
	ht->slot_num=num_slots;										//set num_slots
	ht->item_delete = item_delete;								//link user's delete function
	ht->table = malloc(num_slots * sizeof(set_t *));			//allocate space for table
	if(ht->table == NULL){										//check if malloc success
		fprintf(stderr, "Error in table malloc!\n");
		free(ht);												//if not, free memory
		return(NULL);											//	and return null
	}
	
	for(i=0;i<num_slots;i++){									//allocate each set
		s = set_new(item_delete, NULL);							//pass user's delete function
		if(s==NULL){											//if it failed
			fprintf(stderr, "Error in set malloc!\n");			//	tell user and free each
			for(j=0;j<i;j++){									//	set already malloc'd
				free(ht->table[j]);
			}
			free(ht->table);									//	also free the array
			free(ht);											//	and the whole ht
			return(NULL);										//	and return null
		}
		ht->table[i]=s;											//otherwise, link set to table
	}
	
	return(ht);													//return pointer to ht
}

void *hashtable_find(hashtable_t *ht, char *key)
{
	//get the hashing index from the key string
	unsigned long index = JenkinsHash(key,(unsigned long)sizeof(ht->table)/sizeof(ht->table[0]));
	return(set_find(ht->table[index], key));					//let set_find do the work
																//	and return the result!
}

//return false if key already exists or if ht is null
//copy the key string; don't make a reference to it
bool hashtable_insert(hashtable_t *ht, char *key, void *data)
{
	
	if(ht == NULL){												//check if table is null
		fprintf(stderr, "Error! Hashtable pointer is null.\n");
		return false;											//if so, return false
	}
	
	//check to see if the key is already in the table
	//first, get which set it would be in
	unsigned long index = JenkinsHash(key,(unsigned long)sizeof(ht->table)/sizeof(ht->table[0]));
	
	if(set_find(ht->table[index], key) != NULL){				//check if that set contains the key.
		fprintf(stderr, "Error! Key already exists in hashtable!\n");
		return false;											//if it does, return false
	} else{
		return set_insert(ht->table[index],key,data);			//otherwise, put it in!
	}
	
	return false;												//keep compiler happy
}

void hashtable_delete(hashtable_t *ht)
{
	int i;
	
	if(ht->item_delete == NULL){								//make sure user specified item_delete
		fprintf(stderr, "Error! No item_delete function specified for hashtable structure.\n");
		return;													//if not, exit
	}else if (ht == NULL){										//make sure ht exists
		fprintf(stderr, "Error! Cannot delete nonexistant hashtable.\n");
		return;													//if it does, exit
	} else if (ht->table == NULL){								//make sure the array exists
		fprintf(stderr,"No table malloc'd for the hash function.\n");
		return;													//otherwise, exit
	} else{
		printf("Deleting table sets:\n");
		for(i=0; i < ht->slot_num ; i++){						//iterate through table
			if(ht->table[i] != NULL){							
				set_delete(ht->table[i]);						//delete each set
			}
		}
		free(ht->table);										//delete the array
		free(ht);												//delete whole structure
		printf("Hash table successfully deleted.\n");
	}
}