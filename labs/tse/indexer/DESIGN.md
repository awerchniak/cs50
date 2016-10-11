# Indexer Design
## Requirements Spec (from lab assignment)
```
The TSE indexer is a standalone program that reads the document files produced by the TSE 
crawler, builds an index, and writes that index to a file. Its companion, the index tester,
loads an index file produced by the indexer and saves it to another file.

The indexer shall:

execute from a command line with usage syntax
./indexer pageDirectory indexFilename
where pageDirectory is a directory produced by the crawler, and
where indexFilename is the name of a file into which the index should be written.
read documents from the pageDirectory, each of which has a unique document ID,
wherein the document id starts at 1 and increments for each new document,
and the filename is of form pageDirectory/id,
and the first line of the file is the URL,
and the second line of the file is the depth,
and the rest of the file is the page content.
build an inverted-index data structure mapping from words to (documentID, count) pairs, 
wherein each count represents the number of occurrences of the given word in the given 
document. Ignore words with fewer than three characters, and NormalizeWord before indexing.
create a file indexFilename and write the index to that file, in the format described below.
The indexer shall validate its command-line arguments:

pageDirectory is a directory produced by the crawler (see below), and
indexFilename is the name of a writeable file.
The indexer may assume that

pageDirectory has files named 1, 2, 3, …, without gaps.
The content of files in pageDirectory follow the format as defined in the crawler specs; 
thus your code (to create a WebPage by reading a file) need not have extensive error checking.
The index tester shall:

execute from a command line with usage syntax
./indextest oldIndexFilename newIndexFilename
where oldIndexFilename is the name of a file produced by the indexer, and
where newIndexFilename is the name of a file into which the index should be written.
load the index from the oldIndexFilename into an inverted-index data structure.
create a file newIndexFilename and write the index to that file, in the format described below.
It need not validate its command-line arguments other than to ensure that it receives 
precisely two arguments; it may simply try to open the oldIndexFilename for reading and, 
later, try to open the newIndexFilename for writing. You may use indextest.sh (provided) 
to verify that the output is identical to (or equivalent to) the input.

The index tester may assume that

The content of the index file follows the format specified below; thus your code (to 
recreate an index structure by reading a file) need not have extensive error checking.
```
## Design Spec

### User interface
> indexer pageDirectory indexFilename
> indextest oldIndexFilename newIndexFilename

### Inputs and Outputs
```
Input to indexer: 	The only inputs are the command-line inputs, as shown in user interface.
       				The first is a valid pageDirectory, which is the output from crawler.c. 
       				The second is the name of the file to save our new index to.
Output of indexer: 	Save the index data structure to the specified filename from the command
					line.
Input to indextest:	An index file, previously created by either indexer or indextest,
					as well as a new filename to print the results to.
Output of indextest:An index loaded from the first file and reprinted.
```

### Function decomposition into modules
```
1. Create index.h, a data new data structure that expands on the hashtable data structure.
   This will represent an inverted index, represented by a hashtable of word, counters_t
   pairs, where the counter key is the docID and its count is the number of times the word
   appeared in that document.
   Include the following functions to be usable by external programs:
   1. index_t *index_new(const int num_slots);
   		creates a new index, implemented as a hashtable. Really just calls hashtable_new
   2. bool index_insert(index_t *ht, char *key, counters_t *occurrences);
   		insert into the index. This basically just calls hashtable_insert
   3. counters_t *index_find(index_t *ht, char *key);
   		Return the counters_t paired with the given key in the index
   4. void index_load(index_t *index, char *fileName);
   		load an index_t structure from a saved index file
   5. void index_save(index_t *index, char *fileName);
   		save an index to an index file
   6. void index_delete(index_t *ht);
   		delete the index data structure
   		
   In addition, we will need the following helper programs:
   1. index_delete_helper, which we pass to hashtable_new in index_new to tell the hashtable
   	  data structure how to delete its elements. In our case, we'll simply pass
   	  counters_delete.
   2. index_print_helper, which we pass to hashtable_new in index_new to tell the hashtable
   	  data structure how to delete its elements. In our case, we will basically pass in
   	  counters_print, with perhaps a few extra formatting lines.
   3. (note: added after the fact) I wrote a function, read_ints(), and a helper function for
   	  it, reverse_string(), to extract the docID/frequency pairs from a line read in
   	  index_load() from the index file
2. Create indexer.c, which takes a pageDirectory output from crawler, builds an index data
   structure, and outputs it to indexFilename
   Include the following functions:
   1. main()
   		validate arguments, call index_new(), index_build(), index_save(), and index_delete()
   2. void index_build(index_t *index, char *pageDirectory);
   		build the index data structure from the given pageDirectory containing files from
   		crawler. Make use of the webpage_load() function from webpage.c
   3. void index_page(index_t *index, WebPage *page, int docID);
   		helper for index_build() function. Make use of GetNextWord() from web.c to fill words
   		in the index (after checking that they are at least 3 letters long).
3. Create indextest.c, which takes an output indexFile from indexer, loads it to build an
   index_t data structure, and then saves it to a new indexFile.
   This program is really just a driver, and after parsing the arguments from the command 
   line, simply calls index_new(), index_load(),  index_save(), and index_delete() from 
   index.c
```

### Pseudo code for logic/algorithmic flow
```
1. indexer
	a. Execute from the command line with usage syntax
		./indexer pageDirectory indexFilename
		-where pageDirectory is a valid pageDirectory filled with files from crawler.c
		-where indexFilename is the name of the file to 
	b. Call index_build() to fill the index data structure from the pageDirectory files
		i.	Start with the first docID and loop until the end, using webpage.c's
			webpage_load function to ensure the files exist.
		ii.	For each file, call index_page to fill a WebPage data structure and insert
			the relevant information into the index data structure,  and then call 
			webpage_delete to subsequently free the memory.
		iii.In index_page, use word.c's GetNextWord to parse the html and find each
			word that appears. If it is 3 characters or longer, normalize it and
			insert it into the index data structure. At the end, free all extra memory.
	c. Call index.c's index_save() to save the contents of the filled index to the user's
	   specified file.
	   i.	index_save() basically just calls hashtable_print to that file. The beauty
	   		of data abstraction!
	d. Call index.c's index_delete() to free up all the memory allocated.
	   i.	Data abstraction comes in clutch again; because index_t is really a special
	   		hashtable, index_delete() merely calls hashtable_delete().
2. indextest
	a. Execute from the command line with usage syntax
	   ./indextest oldIndexFilename newIndexFilename
	   -where oldIndexFilename is the output of either a previous indexer or indextest call
	   -where newIndexFilename is the name of the new file to print to.
	   verify that there are the correct number of arguments and that the oldIndexFilename
	   exists
	b. Call index.c's index_new() to create a new index_t data structure
	   i. 	index_new() basically just calls hashtable_new(); data abstraction!
	c. Call index.c's index_load() to load the contents of oldIndexFilename into our new
	   index_t data structure
	   i. 	index_load() will open the file for reading, then scan each line to extract a
	   		key and create a new counters_t for that key.
	   ii.	It then fills that counters_t with the remaining docID, frequency pairs in the
	   		index file.
	d. Call index.c's index_save() to save the contents of our index_t data structure to a
	   new file
	e. Call index.c's index_delete() to free all the memory allocated.
```

### Dataflow through modules
```
1. indexer
	i. 	main() parses command line arguments and calls index_build(), index_save(), and
	   	index_delete().
	ii.	index_build() creates a new WebPage data structure for each docID and calls
		index_page(), before freeing that WebPage
	iii.index_page() parses the HTML and calls NormalizeWord() on any word greater than
		2 characters, before calling index_insert to insert into the index_t
	iv.	NormalizeWord ensures that all words are of the same format
	v.	index_insert basically just calls hashtable_insert!
	vi. index_save() basically just calls hashtable_print!
	vii.index_delete() basically just calls hashtable_delete!
2. indextest
	i.  main() parses command line arguments and calls index_new(), index_load(), 
		index_save(), and index_delete().
	ii.	index_load reads each line using file.h's readline(), and then scans in the key
		and calls read_ints() to find the key, data pairs for the counters_t
	iii.read_ints calls reverse_string to reverse the order of characters in a string
	iv. see above for index_save() and index_delete().
```

### Major Data Structures
> an inverted index, consisting of a hashtable of key, counters pairs, where the keys are
> the words that appear in the HTML, the keys for the counters are the docIDs they appear
> in, and the data for the counters is the frequency at which they occur in those docID's

## Testing Plan
```
Important things to test:
1. Use gdb/valgrind to detect memory leaks
2. indextest.sh is a valuable script. It builds all relevant files using make, takes in a 
   pageDirectory from the command line and calls indexer to build an indexfile from it, then
   calls indextest to load that into an index_t and save it to a new indexfile. After, it
   uses unix commands to sort the files by lines and compare them. If it does not output
   any differences in lines, then we know the index_load() and index_save() functions work
   seamlessly together.
3. Test edge cases, and make sure to never get any segmentation faults!
4. use many "ls" and "cat" commands to ensure that files are made and made correctly.
```
