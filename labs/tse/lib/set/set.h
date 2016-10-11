/* 
 * set.h - header file for set implementation
 *
 * Andy Werchniak, July 2016
 */

#ifndef __SET_H
#define __SET_H

/**************** global types ****************/
typedef struct set set_t;  // opaque to users of the module

/**************** functions ****************/

/*	Create a new empty set
 *	User passes in functions for deleting their desired data type
 * 		as well as printing their data type
 */
set_t *set_new(void (*item_delete)(void *data),void (*print_item)(FILE *fp, char *key, void *data));

/*Find data for for a key within a given set
 *	If not found, return null
 */
void *set_find(set_t *set, char *key);

/*Insert into the set
 */
bool set_insert(set_t *set, char *key, void *data);

/* Delete the set
 *	Note: user must define item_delete function and pass it
 *		to set_new for this to work
 */
void set_delete(set_t *set);

/* Print the set
 * Note: user must define the print_item function and pass it
 * 	to set_new for this to function
 *	MODIFIED FOR TSE TO INCLUDE FILE TO PRINT TO
 */
void print_set(FILE *fp, set_t *set);

/* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void set_iterate(set_t *set, void (*itemfunc)(void *arg, const char *key, void *data), void *arg);

#endif // __SET_H