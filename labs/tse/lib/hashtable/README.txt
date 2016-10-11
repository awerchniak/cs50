README for hashtable.c
Andy Werchniak, July 2016

Compiling:
	gcc -Wall -pedantic -std=c11 -ggdb -o bagtest bagtest.c bag.c
	Or: just type "make"!

Usage:
/* 
 * hashtable.c - hashtable data structure implementation
 * 
 * usage: set of unique string-data pairs indexed with a hashting index
 * stdout: progress messages
 * stderr: error messages
 *
 * Andy Werchniak, July 2016
 */

Assumptions:

- User should never call hashtable_find() after calling hashtable_delete(). 
	hashtable_delete frees the hashtable data structure and all of its elements, and 
	so this will lead to errors. They may not be obvious, but if one analyzes the code 
	with valgrind or similar software, it will become apparent.
- It is the user's responsibility to cast the outputs from hashtable_find appropriately.
	Hashtable works with generic data types, and so it will return a void * pointer. For
	example, if the user has passed a simple integer into hashtable_insert(), when extracting
	that item he/she would need to specify *(int *)hashtable_extract() to read the data
	appropriately.
- hashtable_new() takes two arguments: a size for the table and a pointer to a
	delete function. As hashtable does not know the makeup of the data the user inserts
	into the it, it will not know how to delete it. It is the user's responsibility 
	to create his/her own function to deal with this and pass it to hashtable_new().
	In the most simple case (e.g. a set of string-integer pairs), the delete function may 
	simply be an empty function. However, the functionality is there in case the user 
	wishes to specify more complex data types.
- hashtable_insert() assumes that the key and data arguments are not null. The user may not 
	insert null data into a hashtable.

"Non"-Assumptions:

- The user may choose not to write a delete function for his/her data types. If this 
	is the case, he/she may simply pass (int, NULL) to the hashtable_new() function. 
	However, if he/she makes this choice, there will be memory leaks, as hashtable_new() 
	and hashtable_insert() both call malloc(). In addition, the user will encounter 
	errors if he/she tries to call hashtable_delete().
- The user may use hashtable with any data type he/she wishes.