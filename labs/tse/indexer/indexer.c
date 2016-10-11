/* 
 * indexer - a simple web indexer
 *
 * Andy Werchniak, Summer 2016
 */

/**************** libraries ****************/
#include <stdio.h>							//printf
#include <stdlib.h>							//malloc
#include <string.h>							//string functions
#include <dirent.h>							//opendir()
#include "index.h"							//linked library
#include "counters/counters.h"				//counters_t
#include "hashtable/hashtable.h"			//hashtable
#include "memory/memory.h"					//assertp
#include "web.h"							//given crawler functions, structs
#include "word.h"							//GetNextWord, NormalizeWord
#include "webpage.h"						//webpage_load

/**************** global variables ****************/
														
/**************** local types ****************/

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */

/**************** local functions ****************/
/* not visible outside this file */
/* 	See function definitions for descriptions */
void index_build(index_t *index, char *pageDirectory);
void index_page(index_t *index, WebPage *page, int docID);

/*main()
 *	driver for the program. Simply calls index_new(), index_build(),
 * index_save(), and index_delete().
 */
int main(int argc, char *argv[])
{
	char *pageDirectory, *fileName;
	//0. check parameter
	if(argc != 3){
		fprintf(stderr, "Error! Indexer takes two inputs: pageDirectory and fileName\n");
		return(1);
	} else{
		DIR *dp = opendir(argv[1]);
		if(dp){
			pageDirectory = argv[1];
			fileName = argv[2];
		} else{
			fprintf(stderr, "Please enter a valid pageDirectory.\n");
			return(1);
		}
	}
	
	//1. allocate memory to index data structure
	index_t *index = index_new(10);
	//check if index is NULL
	assertp(index, "Error creating new index.\n");
	
	//2. build the index based on the webpage in the pageDirectory
	index_build(index, pageDirectory);
	
	//3. save the index to an external file fileName
	index_save(index, fileName);

	//4. delete index
	index_delete(index);
	return(0);
}

/*index_build()
 *	fills an index_t data structure from a specified pageDirectory
 *	Based largely on Xia Zhou's notes from Lecture 17-18
 */
void index_build(index_t *index,char *pageDirectory)
{
	WebPage *page;

	//loop through each docID and call index_page
	for(int docID = 1;(page = webpage_load(pageDirectory,docID)) != NULL; docID++){
		//index the html pointed by the page
		index_page(index, page,docID);
		
		//delete page struct
		webpage_delete(page);
	}
}

/*index_page()
 *	insert the contents of a WebPage into an index_t data structure,
 *	normalizing the word and ensuring that it is 3 or more characters
 *	before inserting
 */
void index_page(index_t *index, WebPage *page, int docID)
{
	char* word;
	int pos = 0;

	//start at the first word and move through the entire html
	while((pos = GetNextWord(page->html, pos, &word)) >0){
		//check if word is a trivial word, strlen(word), NormalizeWord
		if(strlen(word)>2)				//ignore words with fewer than 3 characters
			NormalizeWord(word);		//normalize word before indexing
		else{
			free(word);
			continue;
		}
		
		//find the word in hashtable
		counters_t *word_count = index_find(index, word);

		//check if the counter exists
		if(word_count == NULL){
			//allocate memory to create a new counters
			word_count = counters_new();

			//insert the pair of keyword and counter to hashtable
			index_insert(index, word, word_count);
		}

		//now we increment the counter by 1
		counters_add(word_count, docID);

		//free the word
		free(word);
	}
}

