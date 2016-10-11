/* 
 * indextest - load an index, and save it, to test those functions
 *
 * 
 * Andy Werchniak, Summer 2016
 */
 
/**************** libraries ****************/
#include <stdio.h>							//printf
#include <stdlib.h>							//malloc
#include <string.h>							//string functions
#include "index.h"							//linked library
#include "counters/counters.h"				//counters
#include "memory/memory.h"					//assertp

/*main()
 *	only function: simply calls index_new(), index_load(),
 *	index_save(), index_delete()
 */
int main(int argc, char *argv[])
{
	char *oldIndexFilename, *newIndexFilename;
	//0. check parameters
	if(argc != 3){
		fprintf(stderr, "Error! Indexer takes two inputs: oldIndexFilename and newIndexFilename\n");
		return(1);
	} else if(access(argv[1], F_OK) == -1) {
			fprintf(stderr, "Please enter a valid oldIndexFilename.\n");
			return(1);
    } else{
		oldIndexFilename = argv[1];
		newIndexFilename = argv[2];
	}
	
	//create new index
	index_t *index = index_new(10);
	assertp(index, "error allocating index.\n");
	
	//load old index files into index_t data structure
	index_load(index, oldIndexFilename);
	
	//print index_t data structure to new index file
	index_save(index, newIndexFilename);
	
	//always free memory
	index_delete(index);
	return(0);
}