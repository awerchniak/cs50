/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * The index is keyed by words (strings) and stores counter sets; 
 * each counter set represents the set of documents where that word 
 * appeared, and the number of occurrences of that word in each document.
 * 
 * Andy Werchniak, Summer 2016
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "hashtable/hashtable.h"
#include "counters/counters.h"
#include "file.h"

/**************** global types ****************/

// The index is actually a hashtable. 
typedef hashtable_t index_t;

/********** add function prototypes here *********/

/*index_new
 *	create a new index data structure, implemented as hashtable of counters
 *	arguments: number of slots in the table, the user's specified delete function
 *	returns a pointer to the new table
 */
index_t *index_new(const int num_slots);

/*index_insert
 *	insert an element into the index
 *	arguments: index to insert into, key/data pair to insert
 *	return: true if successful, false if unsuccessful.
 */
bool index_insert(index_t *ht, char *key, counters_t *occurrences);

/*index_find
 *	Find the counters with a given key
 *	arguments: index to look in, key to look for
 *	return: the data if found, or null if not
 */
counters_t *index_find(index_t *ht, char *key);

/*index_load
 *	Load the contents of an external index file into an index_t data structure
 *	arguments: index to load into, name of file containing contents
 *	Warning: this will allocate space for each counters_t in the index. User
 *		is responsible for freeing.
 */
void index_load(index_t *index, char *fileName);

/*index_save
 *	Save the contents of an index data structure to an external file
 *	arguments: index to save, name of file to save to
 */
void index_save(index_t *index, char *fileName);

/*index_delete
 *	delete an entire index data structure
 *	arguments: index to delete
 */
void index_delete(index_t *ht);

#endif // __INDEX_H
