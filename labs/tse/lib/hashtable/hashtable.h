/* 
 * hashtable.h - header file for simple bag implementation
 *
 * Andy Werchniak, July 2016
 */

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

/**************** global types ****************/
typedef struct hashtable hashtable_t;  // opaque to users of the module

/**************** functions ****************/

/*hashtable_new
 *	create a new hashtable data structure
 *	arguments: number of slots in the table, the user's specified delete function
 *	returns a pointer to the new table
 */
hashtable_t *hashtable_new(const int num_slots, 
	void (*item_print)(FILE *fp, char *key, void *data),  
	void (*item_delete)(void *data));

/*hashtable_find
 *	find an element with a certain key in the hashtable
 *	arguments: hashtable to look in, key to look for
 *	return: the data if found, or null if not
 */
void *hashtable_find(hashtable_t *ht, char *key);

/*hashtable_insert
 *	insert an element into the hashtable
 *	arguments: hashtable to insert into, key/data pair to insert
 *	return: true if successful, false if unsuccessful.
 */
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

/*hashtable_delete
 *	delete an entire hashtable data structure
 *	arguments: hashtable to delete
 */
void hashtable_delete(hashtable_t *ht);

/*hashtable_print
 *	prints hashtable contents
 *	formatted to work with index.c
 */
void hashtable_print(FILE *fp, hashtable_t *ht);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, 
	void (*itemfunc)(void *arg, const char *key,void *data), 
	void *arg);
		       
#endif // __HASHTABLE_H