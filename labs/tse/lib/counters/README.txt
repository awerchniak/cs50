README for counters.c
Andy Werchniak, July 2016

Compiling:
	gcc -Wall -pedantic -std=c11 -ggdb -o bagtest bagtest.c bag.c
	Or: just type "make"!

Usage:
/* 
 * counters.c : set of integer counters with integer key-count pairs
 * 
 * usage: counters takes an integer input and if there is a counter for that integer
 *	it increments it; else it creates a new one with a count of 1.
 * stdout: progress messages
 * stderr: error messages
 *
 * Andy Werchniak, July 2016
 */

Assumptions:

- User should never call counters_get() after calling counters_delete(). counters_delete
 	frees the counters data structure and all of its elements, and so this will lead to 
 	errors. They may not be obvious, but if one analyzes the code with valgrind or similar
	software, it will become apparent.
- Counters will only deal with positive integers. It only takes integers as inputs, and 
	all of its outputs are positive or zero integers.