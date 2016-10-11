# Querier Design
## Requirements Spec (from lab assignment)
```
The TSE querier is a standalone program that reads the index file produced by the TSE indexer, and answers search queries submitted via stdin.

The querier shall:

1. execute from a command line with usage syntax
	./querier pageDirectory indexFilename
	where pageDirectory is a directory produced by the crawler, and
	where indexFilename is an index file produced by the indexer.
2. load the index from indexFilename into an internal data structure.

3. read search queries from stdin, one per line, until EOF.
	clean and parse each query according to the syntax described below.
	print the ‘clean’ query for user to see.
	use the index to identify the set of documents that satisfy the query, as described below.
	if no documents satisfy the query, print No documents match.
	otherwise, rank the resulting set of documents according to its score, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)
The querier shall validate its command-line arguments:

pageDirectory is a directory produced by the crawler, and
	indexFilename is the name of a readable file.
	The querier may assume that the input directory and files follow the designated formats.

pageDirectory has files named 1, 2, 3, …, without gaps.
	The content of document files in pageDirectory follow the format as defined in the crawler specs; thus your code (to read a document file) need not have extensive error checking.
	The content of the file named by indexFilename follows our Index file format; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
```

## Design Spec

### User interface
```
./querier pageDirectory indexFilename
Keyword: (user enters from stdin)
Query: (echos if valid)
...
```

### Inputs and Outputs
```
Command line arguments: pageDirectory (result of crawler) and indexFilename (result of 
						indexer)
Subsequent inputs: 		User's query (keywords), where 'and' and 'or' are special operators. 
						'and' takes precedence over 'or'. 'and' creates an intersection
						of the hit lists, whereas 'or' creates a union.
Outputs:				Results from the query, ranked by scores (number of times the
						keyword appears in the webpage). 'or' results in a sum of scores,
						whereas 'and' is a minimum.
```

### Function decomposition into modules
```
1.	main() verifies command line arguments, loads the index, and prompts
	the user for queries until it receives EOF (ctrl+D). It then calls
	check()_validity and if successful, it calls print_words() and fills
	an ordered_list of matches for each word. After it has finished testing
	all words, it calls create_master() to create a final list, and then
	list_print() to print it out, before cleaning up the heap memory.
2.	check_validity() checks if the user's query is valid and splits it up
	into an array of words
3.	print_words() prints that query back to the user
4.	create_master() creates a master list from a series of smaller lists,
	where the master list is an intersection of adjacent lists (or lists
	with an 'and' in between them), and a union of lists with an 'or'
	between them. 'and' always takes precedence over 'or. It is similar
	to a tree, where 'ands' represent siblings and 'or's represent parents.
	Below is a diagram:
						  larger master
			/    	union   	|	   	 union		\
		little master	  little master		little master
		  /  int.  \	   /  int.  \			/ intersection \
		this and this or that and that or ...
```

### Pseudo code for logic/algorithmic flow
```
0.	Check if command line arguments are valid (only 2: pageDirectory, indexFilename).
1.	Load a new index_t data structure from the user's specified indexFilename
2.	prompt the user for keywords until EOF
	a.	ensure that the user actually entered something (didn't just press return)
	b.	break the user's entry into an array of words
		i. 	allocate space for the array
		ii. check that the user actually entered words, not just spaces
		iii.check that the user followed all rules regarding operators, non-letters, etc.
		iv.	normalize the words and fill the array
	c.	if the words are valid and the query is good, print it back to the user
	d.	look at each word individually & check if it's an operator
	e.	find the docID's that contain the words and their frequencies
		i.	index_find gives us a counter where the keys are the docID's and the
			data is their frequencies
		ii.	Implement new data structure, an ordered linked list, to keep track
			of the documents sorted by frequency
		iii.for each word, create an ordered list of the documents that contain it
			(using index_find). Also create a master ordered list. If no operators
			appear between the words, this master ordered list is the intersection of
			all the little lists
		iv.	If the user types 'and', we can ignore it (treat two consecutive words
			the same as an 'and')
		iv.	if the user types 'or', the master list becomes the union of the two sets,
			where repeated docID's have their frequencies summed.
		v.	AND takes precedence over OR
3.	cleanup
```

### Dataflow through modules
```
1.	main() verifies the command line arguments, loads the index, and takes queries from
	stdin. For each query, it creates an array of strings (one per word), and passes that
	to check_validity(). Each query gets an array of ordered_lists, and each word gets its
	own list (filled inside of counters_iterate()). Next, main() calls create_master to make
	one big list, list_print() to display it, and then cleans up.
2.	check_validity() both checks the user's query and fills an array of strings, one per 
	word using strtok(). It also calls NormalizeWord() to make all of the words lowercase
3.	We pass catalog_doc() to counters_iterate(), and it performs its functionality on each
	counter in the counters for that word. catalog_doc() inserts each hit_t structure into
	the ordered_list passed to it.
4.	create_master() takes all of the mini lists created for each word and combines them 
	into a master list, using unions for 'or' and intersections for 'and', where 'and'
	takes precedence over 'or'
5.	list_print() prints the results of the query back to the user, which are in order by
	frequency as list_insert() automatically sorted them back in catalog_doc().
```

### Major Data Structures
> ordered_list is a linked list of docID, score, url groups sorted in descending
> order based on scores. This implementing this data structure vastly simplifies
> the task of sorting the matching files in order for the user to see. For this reason,
> querier treats all matches as ordered_lists, so as to never have to worry about ordering
> them correctly.

## Testing Plan
```
1.	Test the ordered_list data structure individually. Test edge cases for each function
	and use valgrind to detect memory leaks. Do this before implementing in querier to
	catch and isolate bugs early.
2.	Test edge cases with command line arguments (pageDirectory, indexFilename)
3.	Test edge cases for keywords input (just a '\n', just spaces, extra spaces between
	words, etc.)
4.	Try to make it segfault using any means (incredibly long queries, many 'and' and 'or'
	operators, etc.
5.	Always valgrind to check for memory leaks!
```
