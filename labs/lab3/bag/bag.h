/* 
 * bag.h - header file for simple bag implementation
 *
 * Andy Werchniak, July 2016
 */

#ifndef __BAG_H
#define __BAG_H

/**************** global types ****************/
typedef struct bag bag_t;  // opaque to users of the module

/**************** functions ****************/

/* Create a new (empty bag)
 * return NULL if error.
 * User enters his/her own defined functions for deleting and printing
 */
bag_t *bag_new(void (*item_delete)(void *data), void (*print_item)(void *data));

/* Insert an item into the bag
 */
void bag_insert(bag_t *bag, void *data);

/*	Remove an item from the bag and return it to the caller
 *	Return NULL if the bag is empty
 */
void *bag_extract(bag_t *bag);

/*Delete all of the items in the bag and free its memory
 *	User must specify function and pass to bag_new for this to work
 */
void bag_delete(bag_t *bag);

/*Print out all of the items in the bag
 *	User must specify function and pass to bag_new for this to work
 */
void print_bag(bag_t *bag);
#endif // __BAG_H