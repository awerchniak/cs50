/* 
 * counters.h - header file for counters data structure
 *
 * Andy Werchniak, July 2016
 */

#ifndef __COUNTER_H
#define __COUNTER_H

/**************** global types ****************/
typedef struct counters counters_t;  // opaque to users of the module

/**************** functions ****************/

/* counters_new:
 * 	Create a new empty counters list
 *	User passes in functions for deleting their desired data type
 * 		as well as printing their data type
 */
 counters_t *counters_new(void);

/* counters_add:
 * 	If a key exists, increment its counter
 *	If not, create one and set its counter to 1
 *	arguments: counters data structure, key to work on
 */
void counters_add(counters_t *ctrs, int key);

/* counters_get:
 * 	Return value of counter with given key
 *		If not found, return 0
 *	arguments: counters data structure, key to get
 */
int counters_get(counters_t *ctrs, int key);

/* Print all counters, simply as key and then count
 *	works well with index_save!
 */
void counters_print(FILE *fp, counters_t *ctrs);

/* counters_delete:
 * 	Delete the set of counters
 *	argument: structure to delete
 */
void counters_delete(counters_t *ctrs);

/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 */
void counters_set(counters_t *ctrs, int key, int count);

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs, void (*itemfunc)(void *arg, const int key, int count), void *arg);

#endif // __COUNTER_H