# querier.c

## Functionality
> One of three major components in the Tiny Search Engine, 16X
> Given a pageDirectory (the output of crawler.c; see 
> ../crawler/README.md for more information), and an indexFilename
> (see ../indexer/README.md for more information), fills an index_t
> data structure and processes the user's queries from the command
> command line, until it receives EOF (ctrl+D).
>
> The user may employ the operators 'and' and 'or', in queries.
> 'and' takes precedence over 'or', and an 'and' returns a list that
> is the intersection of the two lists, whereas 'or' returns the union.
>
> Results are ranked in order of score

## Compiling
      make
## cleanup
      make clean

## Usage example
      ./querier pageDirectory indexFilename
      Keywords: this and these or that and those

## Exit codes
      0.	No problems- program runs smoothly
      1.	Invalid arguments passed from command line upon init
      99.	Error with malloc

## Assumptions
      1. 	The user's queries should not be more than 1000 characters. If they are, querier
      		will simply break them up into two separate queries at the 1000 character mark.
      2.	The user's input may not contain any non-alphabetical characters (aside from
      		spaces or tabs), the user may not begin or end queries with 'and' or 'or' 
      		operators, and the user may not use consecutive operators.
      3.	pageDirectory and indexFilename, passed from the command line upon calling
      		querier, are valid outputs from crawler.c and indexer.c, respectively. This
      		means that pageDirectory contains files named pageDirectory/1 ... pageDirectory/n
      		where the first line is the url. indexFilename is a file that contains one
      		word per line followed by numbers representing the files that word occurs in
      		and the frequency at which it occurs.
      4.	url's listed in the documents in pageDirectory are not over 1000 characters
      		(otherwise they will be truncated).
      5.	All paths pageDirectory/n must be under 100 characters. That means, for example,
      		that if the pageDirectory contains 10^3 files, it must be under 95 characters
      		long.

## Non-Assumptions
      1.	The user may specify a pageDirectory with any number of files, so long as 
      		the resulting index is not large enough that the heap runs out of memory!
      2.	The user may specify an indexFilename with any number of lines, so long as 
      		it is sufficiently small that the heap does not run out of memory.
      3.	The user may use any number of operators in his query, so long as they make
      		sense. This means that queries may not begin or end with operators, and 
      		operators may not appear consecutively.
      4.	The user may search for any word he/she wishes, so long as it consists only
      		of alphabetical characters. 

## Limitations
      1.	Querier cannot search for URLs containing the words and' or 'or', as they are 
      		special operators with special functionality.
