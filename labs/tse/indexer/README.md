# indexer.c, indextest.c

## Functionality
> One of three major components in the Tiny Search Engine, 16X
> Given a pageDirectory (the output of crawler.c; see 
> ../crawler/README.md for more information), fills an index_t
> data structure (part of ../common/index.h), pairing words
> with the files they occur in and the frequency at which they
> occur in each file, in an inverted index. 
>
> Indextest exists to test the functionality of index_load() and
> index_save() in the index_t data structure. It simply loads
> an index from a previously saved file and then outputs it into
> a new file.

## Compiling
      make
## cleanup
      make clean

## Usage example
      ./indexer ../data/pageDirectory indexFilename
      
      ./indextest oldFilename newFilename

## Exit codes
      0.	No problems- program runs smoothly
      1.	Invalid pageDirectory passed from command line
      99.	Error with malloc

## Assumptions
      1. 	Indexer will allow the user to input an already existing indexFilename,
      		but it would be ill-advised to do so. In this case, indexer will simply
      		append to the end of that file. If the user later tries to load in that
      		index using index_load() from index.c, it will not be identical, as the
      		earlier lines will also be interpreted as a part of the data structure.
      2.	Indexer may assume that pageDirectory is a valid pageDirectory with files
      		named 1,2,3, etc. without gaps, and that those files follow the index format
      		(see item 4).
      3.	Indextest takes in a valid oldIndexFilename, which was purely the output
      		from indexer. Similarly to indexer, indextest may output to any already
      		existing file, although this is not advised.
      4.	Indextest assumes that the oldIndexFilename parameter is a file that follows
      		the index format, meaning: 
      			one line per word, one word per line
				each line provides the word and one or more (docID, count) pairs, in the 
					format: word docID count [docID count]…
				where word is a string of lower-case letters,
				where docID is a positive non-zero integer,
				where count is a positive non-zero integer,
				where the word and integers are separated by spaces.
			If negative values appear on the lines, index_load() will ignore the 
			negative signs and interpret them as positive numbers.

## Non-Assumptions
      1.	The user may specify a pageDirectory with any number of docID's for indexer,
      		so long as the index does not become so large that the system runs out of 
      		memory.
      2.	The user may load an oldIndexFilename of any length into indextest, so long
      		as the system does not run out of memory.

## Limitations
      1.	index_load() will interpret apostrophes as the end of a word. This is a result
      		of a bug in the GetNextWord function provided in the word.h library
      2.	heap memory of the machine it runs on!
