/*
 *	crawler.c - crawl a website to find related sites
 *		Implemented using Xia Zhou's format and web files
 *		For CS50, 16X
 *
 *	Andy Werchniak, July 2016
 */
 
/**************** libraries ****************/
#include <unistd.h>							//access()
#include <stdbool.h>						//bool type
#include <ctype.h>							//isdigit()
#include <stdio.h>							//printf
#include <stdlib.h>							//malloc
#include <string.h>							//string functions
#include "web.h"							//given crawler functions, structs
#include "bag/bag.h"						//bag data structure
#include "hashtable/hashtable.h"			//hashtable data structure
#include "memory/memory.h"					//given memory functions

/**************** global variables ****************/
														
/**************** local types ****************/

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */

/**************** local functions ****************/
/* not visible outside this file */
/* 	See function definitions for descriptions */
void crawl(char *seedURL, char *pageDirectory, int maxDepth);
WebPage *webpage_new(char *url, int depth);
bool page_save(WebPage *web, char *pageDirectory, int docID);
bool page_scanner(WebPage *web, bag_t *pages_to_fetch, hashtable_t *pages_seen);
bool IsWritableDirectory(char* direc);
bool IsNumericString(char *string);
void web_delete(void*data);
void url_delete(void *url);
inline static void logr(const char *word, const int depth, const char *url);

/*	main() - driver for crawler.c
 *		parameters: arguments and # of arguments from command line
 *		returns: 0 if successful, other if not
 *		parses the inputs and checks if they are valid. If so, calls crawl()
 */
int main(int argc, char *argv[])
{
	//0. if argc != 4
	if(argc != 4){
		fprintf(stderr, "Please enter three arguments: seedURL, pageDirectory, and maxDepth.\n");
 		return(1);
	}
	
	//1. seed URL
	char *seedURL = argv[1];
	if(!IsInternalURL(seedURL)){				//this function calls normalize
		fprintf(stderr, "Please enter a valid, internal seedURL.\n");
		return(1);
	}
	
	//2. page directory
	char *pageDirectory = argv[2];
	if(!IsWritableDirectory(pageDirectory)){
		fprintf(stderr, "Please enter a valid, writable pageDirectory.\n");
		return(1);
	} else{
		//if there is a directory, create a file named ".crawler" so we know
		//	this was a crawler directory
		char filename[100];
		sprintf(filename, "%s/%s", pageDirectory, ".crawler");
		FILE *fp = fopen(filename, "w");
		fclose(fp);
	}

	//3. maxDepth
	char *depthString = argv[3];
	int maxDepth;
	if(!IsNumericString(depthString) || atoi(depthString)<0){
		fprintf(stderr, "Please enter a valid, numeric maxDepth.\n");
		return(1);
	} else maxDepth = atoi(depthString);

	crawl(seedURL, pageDirectory, maxDepth);
	
	return(0);
}

/*	crawl() - the meat of crawler.c
 *		parameters: validated seedURL, pageDirectory, and maxDepth from command line
 *		crawls the seedURL, saving each valid, internal URL to a file in the pageDirectory
 *		Only crawls up until the maxDepth
 */
void crawl(char *seedURL, char *pageDirectory, int maxDepth)
{
	WebPage *web;
	int docID = 0;
	
	//0. allocate space for data structure bag and hashtable
	//		ensure that malloc exited smoothly
	bag_t *pages_to_fetch = bag_new(web_delete, NULL);
	assertp(pages_to_fetch, "bag malloc failed.\n");
	
	hashtable_t *pages_seen = hashtable_new(10,NULL,url_delete);
	assertp(pages_seen, "hashtable malloc failed.\n");
	//key in the hashtable is URL, data is NULL


	//1. insert seedURL page into the bag
	bag_insert(pages_to_fetch, webpage_new(seedURL,0)); 

	//2. loop through all the web page objects in the bag
	while ((web = bag_extract(pages_to_fetch)) != NULL) {
		logr("Fetched", web->depth, web->url);
		
		//fetch the page, and make sure it can be parsed!!
		if(!GetWebPage(web)){
		
			//if not, output error & extract next bag item
			logr("Error with URL", web->depth, web->url);
			web_delete(web);			//remember to free the page!
			
			//if the next item is null, we are done
			if((web = bag_extract(pages_to_fetch)) == NULL)
				break;
			else{
				//otherwise, continue!
				logr("Fetched", web->depth, web->url);
			}
		}
			
		//save it to the page directory
		if(page_save(web, pageDirectory, ++docID))
			logr("Saved", web->depth, web->url);
		else logr("Error saving", web->depth, web->url);

		//scan for more URLS
		if(web->depth < maxDepth){			
			//use hashtable to decide if page has already been seen
			logr("Scanning", web->depth, web->url);
			page_scanner(web, pages_to_fetch, pages_seen);
		}

		//delete the web page object
		web_delete(web);
	}

	//3. clean up
	hashtable_delete(pages_seen);
	bag_delete(pages_to_fetch);
}

/*	webpage_new() - allocate and create a new WebPage structure
 *		parameters: url and depth of the webpage
 *		returns: a pointer to that WebPage structure or NULL upon failure
 */
WebPage *webpage_new(char *url, int depth)
{
	//create a new WebPage structure and ensure the malloc is successful
	WebPage *new = malloc(sizeof(WebPage));
	assertp(new, "WebPage malloc failed.\n");
	
	//set the depth
	new->depth = depth;
	
	//malloc space for url, ensure malloc is successful, and copy over URL
	new->url = (char *)malloc(sizeof(char)*(strlen(url)+1));
	assertp(new->url, "url malloc failed.\n");
	strcpy(new->url, url);
	
	//initialize html to null (GetWebPage will fill this)
	new->html = NULL;
	
	//return a pointer to the new WebPage structure
	return(new);
}

/*	page_save() - save a WebPage to a file in the specified directory
 *		parameters: WepPage structure to save, pageDirectory to save it into,
 *					and docID to name the file
 *		returns: true if successful, false if error
 */
bool page_save(WebPage *web, char *pageDirectory, int docID)
{
	//create a buffer for the filename
	char filename[100];			//give it a decent sized buffer just in case
	
	//name if pageDirectory/docID
	sprintf(filename, "%s/%d", pageDirectory, docID);
	
	//make sure this filename doesn't exist
	if(access(filename, F_OK) == -1){
		//if it doesn't exist, create a new file for writing
		FILE *fp = fopen(filename, "w");
	
		//write url, depth, and html to it
		fprintf(fp, "%s\n%d\n%s", web->url, web->depth, web->html);
		fclose(fp);			//always close the file
		
		//return success
		return(true);
	} else return(false);	//otherwise, return failure
}

/*	page_scanner() - scan a WebPage's html to find more URLs
 *		parameters: WebPage to scan, bag to insert pages into, hashtable to 
 *					insert URLs into
 *		returns: true if successful, false if error
 */
bool page_scanner(WebPage *web, bag_t *pages_to_fetch, hashtable_t *pages_seen)
{
	char *url;
	int pos = 0;

	//scan while there are more URLs
	while ((pos = GetNextURL(web->html, pos, web->url, &url)) > 0) {
		
		//check if the url is internal
		//check if it’s been seen before using hashtable_insert
		if(IsInternalURL(url) && hashtable_insert(pages_seen,url,url)){
		
			//if it’s a new internal URL, create WebPage object, insert to bag
			logr("Found", web->depth, url);
			bag_insert(pages_to_fetch, webpage_new(url,web->depth+1));
			logr("Added", web->depth, url);
		
		//if it's not internal, ignore it
		} else if (!IsInternalURL(url))
			logr("IgnExtrn", web->depth, url);
		
		//clean up
		free(url);
	}
	
	return(true);
}

/*	IsWritableDirectory(): check if a directory is writable
 *		parameters: string name (path) of directory
 *		returns: true if writable, false if not
 */
bool IsWritableDirectory(char* direc) 
{
	if(!access(direc, W_OK))
		return true;
	else
		return false;   
}

/*	IsNumericString(): check if a string only consists of integers
 *		parameters: string to check
 *		returns: true if this string could be an int, false if not
 *
 */
bool IsNumericString(char *string){
	//look at each element in the string and check if it's a digit
	for(int i = 0; string[i] != '\0'; i++){
		if(!isdigit(string[i]))
			//if any aren't digits, return false
			return false;
	}
	
	return true;
}

/*	web_delete(): delete a WebPage structure and frees its memory
 *		parameters: WebPage to delete (void* so we may pass to bag_delete)
 */
void web_delete(void *data)
{
	WebPage *web = data;

	//check if they aren’t null first
	//if not, free all the elements
	if(web != NULL && web->url != NULL){
		free(web->url);
		web->url = NULL;
	}
	if(web != NULL && web->html != NULL){
		free(web->html);
		web->html = NULL;
	}
	if(web != NULL){
		free(web);
		web = NULL;
	}
}

/*	url_delete(): delete the data stored in data with URL key
 *		for use with hashtable_delete
 */
void url_delete(void *url)
{
	//empty function: no data passed into hashtable
}

/* logr(): function to help format printing. Written by Xia Zhou
 *		parameters: word to print, depth of item printed, url to print
 */
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}