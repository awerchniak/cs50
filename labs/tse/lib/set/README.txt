README for set.c
Andy Werchniak, July 2016

Compiling:
	gcc -Wall -pedantic -std=c11 -ggdb -o bagtest bagtest.c bag.c
	Or: just type "make"!

Usage:
/* 
 * set.c - set data structure implementation
 * 
 * usage: set of unique string-data pairs
 * stdout: progress messages
 * stderr: error messages
 *
 * Andy Werchniak, July 2016
 */

Assumptions:

- User should never call set_find() after calling set_delete(). set_delete frees
	the set data structure and all of its elements, and so this will lead to errors.
	They may not be obvious, but if one analyzes the code with valgrind or similar
	software, it will become apparent.
- It is the user's responsibility to cast the outputs from set_find appropriately.
	Set works with generic data types, and so it will return a void * pointer. For
	example, if the user has passed a simple integer into set_insert(), when extracting
	that item he/she would need to specify *(int *)set_extract() to read the data
	appropriately.
- set_new() takes two arguments: a pointer to a print function and a pointer to a
	delete function. As set does not know the makeup of the data the user inserts
	into the set, it will not know how to print it or delete it. It is the user's
	responsibility to create his/her own functions to deal with this and pass them
	to set_new(). In the most simple case (e.g. a set of string-integer pairs), the 
	print function may just be a single printf, and the delete function may simply 
	be an empty function. However, the functionality is there in case the user wishes 
	to specify more complex data types.
- set_insert() assumes that the key and data arguments are not null. The user may not 
	insert null data into a set.

"Non"-Assumptions:

- The user may choose not to write delete or print functions for his/her data types.
	If this is the case, he/she may simply pass (NULL, NULL) to the set_new() function. 
	However, if he/she makes this choice, there will be memory leaks, as set_new() and
	set_insert() both call malloc(). In addition, the user will encounter errors if
	he/she tries to call set_delete() or set_print().
- The user may use set with any data type he/she wishes.