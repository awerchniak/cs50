/*
 *	querier.c - program to take user's queries from command line
 *		and return webpages that contain those keywords, ranked
 *		by frequencies at which the keywords occur.
 *
 *		supports 'and' and 'or' operators, where 'and' scores are
 *		the minimum scores from the two queries, and the 'or' 
 *		scores are the sum.
 *
 *		The third part of the tiny search engine, to be used in
 *		conjunction with crawler.c and indexer.c
 *
 *	Andy Werchniak, August 2016
 *
 */

/**************** libraries *********************/
#include "index.h"						//index_t
#include "word.h"						//NormalizeWord()
#include <stdbool.h>					//bool type
#include <stdlib.h>						//malloc()
#include <stdio.h>						//printf, etc.
#include <string.h>						//strcmp, strcpy, etc.
#include <ctype.h>						//isalpha()
#include <unistd.h>						//access()
#include <dirent.h>						//opendir()

/*********** preprocessor declarations ***********/
#define URL_LEN 1000					//no url may be more than 1000 characters
#define QUERY_LEN 1000					//no query may be more than 1000 characters
#define PATH_LEN 100					//no pageDirectory/docID path may be more than
										//	100 characters
/************** new data structure ***************/
/*	hit_t : a structure to represent relevant information from a file that contains
 *		a given keyword. These also are the individual elements in the ordered_list
 *		(see below).
 *
 *	Limitations: can only deal with URL's of under 1000 characters
 */
typedef struct hit{
	int docID;
	int frequency;
	char url[URL_LEN];
	struct hit *next;
} hit_t;

/*	ordered_list : a new data structure created just for querier
 *		A sorted linked list of hit_t structures, where elements appear
 *		in descending order based on frequencies. The hit_t structure
 *		with the highest frequency comes first, and the others follow.
 *		This is useful for printing query results in descending order.
 *		
 *		For convenience, it also contains a copy of the pageDirectory name,
 *		in case a function using it would like to access it.
 *
 *	Limitations: can only deal with pageDirectory names under 100 characters
 *	
 */
typedef struct{
	hit_t *head;
	char pageDirectory[PATH_LEN];
} ordered_list;

/***** functions to implement data structure *****/
/*list_new : create a new list
 *	arguments: string containing the name of the pageDirectory where the 
 *		querier will look
 *	returns: a pointer to the new list
 *
 *	NOTE: list_new allocates memory for the new ordered_list and the user
 *		is responsible for freeing that memory afterwards.
 */
ordered_list *list_new(char *pageDirectory)
{
	
	//ensure we don't exceed char array bounds
	if(pageDirectory!= NULL && strlen(pageDirectory) > PATH_LEN -1){
		fprintf(stderr, "Please enter a pageDirectory under %d characters", PATH_LEN);
		return(NULL);
	}
	
	//malloc space
 	ordered_list *new = malloc(sizeof(ordered_list));
 	
 	if(new == NULL){									//check if malloc failed
 		fprintf(stderr, "Error with malloc! No list created.\n");
 		return(NULL);
 	}else{												//otherwise, set head to NULL
 		new->head = NULL;								//and return pointer
 		if(pageDirectory != NULL)
 			strcpy(new->pageDirectory, pageDirectory);	//copy pageDirectory in
 		return(new);
 	}
}

/* list_peak : peek into the list to see if it contains an element
 *	with a given key
 *
 *	arguments: list to peek in, key to look for
 *	returns: pointer to hit_t element if found, NULL if not
 */
hit_t *list_peek(ordered_list *list, int key)
{
	if(list-> head == NULL){
		return(NULL);
	} else{
		for(hit_t *ptr = list->head; ptr != NULL; ptr=ptr->next){
			if(ptr->docID == key)
				return(ptr);
		}
		return(NULL);
	}
}

/* list_remove : remove a hit_t element with a given key from
 *		and ordered_list.
 *	arguments:list to remove from, docID of element to remove
 *
 *	returns: true if successful, false if unsuccessful.
 */
bool list_remove(ordered_list *list, int key)
{
	if(list == NULL || list->head == NULL){
		fprintf(stderr, "Empty list.\n");
		return(false);
	}
	
	hit_t *temp;
	
	//check the head first
	if(list->head->docID == key){
		temp = list->head;
		list->head = list->head->next;
		free(temp);
		return(true);
		
	} else{
		int found =  0;
		for(hit_t *ptr = list->head; ptr->next != NULL; ptr=ptr->next){
			if(ptr->next->docID == key){
				found = 1;
				temp = ptr->next;
				ptr->next = ptr->next->next;
				free(temp);
				return(true);
			}
		}
		
		if(!found){
			fprintf(stderr, "docID '%d' not in the list.\n", key);
			return(false);
		}
	}
	
	return(false);
}

/* list_insert : insert a new hit_t element into an ordered_list.
 *		Will insert in the descending order, based on frequency.
 *		If the user inserts an item into the list that is already
 *		there, list_insert will set the frequency to their sums
 *
 *	arguments: list to insert into, and docID, frequency, and url of new
 *		hit_t structure
 *
 */
void list_insert(ordered_list *list, int docID, int frequency, char *url)
{
	//ensure we don't exceed char array bounds
	if(strlen(url) > URL_LEN -1){
		fprintf(stderr, "Please enter a url under %d characters", URL_LEN);
		return;
	}
	
	int sum = 0;
	hit_t *temp;
	
	if((temp = list_peek(list,docID))!=NULL){
		sum = temp->frequency;
		list_remove(list, temp->docID);
	}
		
	hit_t *new = malloc(sizeof(hit_t));
	if(new == NULL){
		fprintf(stderr, "Error with malloc.\n");
		return;
	}
	
	//fill hit_t *new
	new->docID = docID;
	new->frequency = sum + frequency;
	strcpy(new->url,url);
	
	if(list->head == NULL){
		new->next = NULL;
		list->head = new;
	}else if((list->head->frequency) < (new->frequency)){
		new->next = list->head;
		list->head = new;
	}else{
		//find where to insert the new one in order
		hit_t *ptr;
		
		for(ptr = list->head; ptr->next!= NULL && (ptr->next->frequency) > (new->frequency); ptr=ptr->next);
		
		new->next = ptr->next;
		ptr->next = new;
	}
}

/* list_extract : extract the hit_t structure with the highest frequency
 *	arguments : list to extract from
 *	returns : hit_t structure with highest frequency
 *
 *	NOTE: list_extract allocates memory for the hit_t structure it returns.
 *		The user is responsible for freeing that structure later on.
 */
hit_t *list_extract(ordered_list *list)
{
	if(list == NULL){
		fprintf(stderr, "Error! Cannot extract from nonexistent list.\n");
		return(NULL);
	}
	
	if(list-> head == NULL){
		return(NULL);
	} else{
		hit_t *hit = malloc(sizeof(hit_t));
		hit_t *temp = list->head;
	
		hit->docID = temp->docID;
		hit->frequency = temp->frequency;
		strcpy(hit->url, temp->url);
	
		list->head = temp->next;
		free(temp);
		return(hit);
	}
}

/* list_append : copy the contents of one list into another
 *		can be used to copy into an empty list, or create a
 *		union from an already existing list
 *
 *	arguments:	list to copy into, list to copy from
 *	returns:	true if successful, false if not
 *	assumptions: new list has already been malloc'd (list_new())
 */
bool list_append(ordered_list *new, ordered_list *old)
{
	if(new == NULL){
		//fprintf(stderr, "Cannot copy into un-malloc'd list.\n");
		return(false);
	} else if(old == NULL){
		//fprintf(stderr, "Cannot append an un-malloc'd list!.\n");
		return(false);
	}
	
	for(hit_t *ptr = old->head; ptr!= NULL; ptr=ptr->next)
		list_insert(new,ptr->docID,ptr->frequency,ptr->url);
	
	return(true);
}

/* list_intersect : create an intersection of two lists
 *	arguments: two lists to create intersection of
 *	returns: pointer to new intersection list
 *
 *	warning: this creates a new list. User is responsible for
 *	freeing all memory
 */
ordered_list *list_intersect(ordered_list *master, ordered_list *slave)
{
	if(master == NULL || slave == NULL){
		fprintf(stderr, "Error! one of the two lists is not malloc'd.\n");
		return(master);
	}
	
	int freq, master_freq, slave_freq;
	ordered_list *new = list_new(slave->pageDirectory);
	
	//start at the head of master
	//for each element, check if it's also in slave
	for(hit_t *ptr = master->head; ptr != NULL; ptr = ptr->next){
		if(list_peek(slave, ptr->docID) == NULL){
			//printf("delete: %d\n", ptr->docID);
		} else{
		 	//printf("keep: %d\n", ptr->docID);
		 	master_freq = ptr->frequency;
		 	slave_freq = list_peek(slave, ptr->docID)->frequency;
		 	freq = (master_freq < slave_freq) ? master_freq : slave_freq;
		 	list_insert(new, ptr->docID, freq, ptr->url);
		}
	}

	//if we want to keep it, insert it into a new ordered_list
	//at the end, delete the old master, and set master to the new ordered_list
	return(new);
}

/* list_print : print the elements in an ordered_list structure in order
 *	arguments: list to print
 */
bool list_print(FILE *fp, ordered_list *list)
{
	if (list == NULL) {
		fputs("(null)\n", fp);
		return false; // bad list
	} else {
		if(list->head == NULL){
			fprintf(fp, "No documents match.\n");
			return false;
		}
		
		// scan the list
		for (hit_t * node = list->head; node != NULL; node = node->next) {
			// print the current node
			fprintf(fp, "score %3d doc %4d: %s", node->frequency, node->docID, node->url);
		}
		return(true);
	}
	return false;
} 

/* list_delete : delete an entire ordered_list data structure
 *	arguments: list to delete
 *
 *	non-assumptions: list may be null (in that case, list_delete does nothing)
 */
void list_delete(ordered_list *list)
{
	if(list == NULL){
		//fprintf(stderr, "Cannot delete nonexistent list.\n");
		return;
	} else if(list->head == NULL){
		free(list);
		return;
	}
	
	hit_t *temp, *ptr;								
 	ptr = list->head;									//start at the first hit
 	
	while(ptr != NULL){									//go until end of list
		temp=ptr->next;									//temp pointer so we can move on
		free(ptr);										//free the data in that spot
		ptr = temp;										//move onto next one
	}

	free(list);											//free the entire data structure
}

/* create_master : create a master list from an array of smaller lists
 *	arguments:	pageDirectory : string passed to list_new()
 *				lists: array of lists to unify,
 *				operators[]: an array of integers representing the operator status
 *					of each word. Operators[i] = 0 for non-operators, 1 for 'and', 
 *					and 2 for 'or'.
 *				len : numbers of lists to unify
 *	returns: new master list created from the smaller lists
 *
 *	limitations: the user may pass empty lists in the *lists[] array, but the lists
 *		must at least be malloc'd (list_new() must be called on every list prior to
 *		passing the array to create_master()).
 */
ordered_list *create_master(char *pageDirectory, ordered_list **lists, int *operators, int len)
{
	//create a new master list
	ordered_list *master = list_new(pageDirectory);

	//the master list starts out as just the first list
	//	if it doesn't work, just return this empty list
	if(!list_append(master, lists[0])){
		fprintf(stderr, "Error! master list corrupted.\n");
		return(master);
	}
	
	for(int i = 1; i<len; i++){
		//start making a list as intersections of the lists
		
		//if the word is not an operator, just list_intersect
		if(!operators[i]){	
			ordered_list *temp = master;
			
			master = list_intersect(master, lists[i]);
				
			list_delete(temp);
			
		//if we encounter an 'or', begin a sublist
		} else if(operators[i] ==2) {

			ordered_list *sublist = list_new(pageDirectory);
			list_append(sublist, lists[++i]);
			
			//within that sublist, everything is intersections
			//go until the end or another 'or'
			while(i<len && operators[i]!=2){
				if(operators[i] != 1){
					ordered_list *temp = sublist;
					sublist = list_intersect(sublist,lists[i]);
					list_delete(temp);
				}
				
				i++;
			}
			
			//bug fix: on second 'or', we don't want to skip over the 'or'!!!!
			if(operators[i] == 2)
				i--;
			
			//at the end, make master into a union of itself and the sublist
			list_append(master,sublist);
			
			//cleanup
			list_delete(sublist);
			
		}
		
	}

	return(master);
}

/**************** local functions ****************/
/* not visible outside this file */
/* 	See function definitions for descriptions */
static int check_validity(char *buffer, int *num_words, char **words);
static void catalog_doc(void *arg, const int key, int count);
static bool is_operator(char *word);
static bool contains_chars(char *buffer);
static void free_words(int length, char **words);
static void print_words(int length, char **words);

//assumptions: 	command line query cannot be greater than 1000 characters
//				pageDirectory is valid
//				indexFilename is valid
//				no url is greater than 1000 characters
int main(int argc, char *argv[])
{
	//check parameters from command line
	if(argc != 3){
		fprintf(stderr, "querier takes two inputs: pageDirectory and indexFilename.\n");
		return(1);
	}
	
	//make sure the page directory exists
	DIR *pageDirectory = opendir(argv[1]);
	if(!pageDirectory){
		fprintf(stderr, "pageDirectory does not exist.\n");
		closedir(pageDirectory);
		return(1);
	} closedir(pageDirectory);
	
	//make sure the directory contains ".crawler"
	char filename[PATH_LEN];
	sprintf(filename, "%s/%s", argv[1], ".crawler");
	if(access(filename, F_OK) == -1){
		fprintf(stderr, "Please entry a valid pageDirectory - the product of crawler.\n");
		return(1);
	}
	
	//make sure the index file exists and is readable
	if(access(argv[2], F_OK) == -1){
		fprintf(stderr, "indexFilename does not exist.\n");
		return(1);
	}
	
	//load index from indexFilename
	index_t *index = index_new(10);
	index_load(index, argv[2]);
	
	counters_t *counters;			//pointer for later
	
	char buffer[QUERY_LEN];			//the user's query may only be 1000 characters long
	int num_words = 0;
	
	//scan from the command line until the user presses ctrl+D
	printf("Keywords: ");
	while(fgets(buffer, QUERY_LEN, stdin) != NULL){
	
		//remove the carriage return at the end of the line
		buffer[strlen(buffer)-1] = '\0';
		
		//make sure the user didn't just press enter, to avoid segfault
		if(strlen(buffer) == 0){
			fprintf(stderr, "Please enter a query with a length greater than 0.\n");
			printf("-------------------------------\nKeywords: ");
			continue;
		}
		
		//otherwise, create an array of the words the user entered
		//length of words is the strlen of buffer
		char **words = malloc(sizeof(char*) * strlen(buffer));
		
		//fill the words array and make sure the user's input is valid
		//	this function also fills the words[] array, where each slot
		//	is one of the words (after being normalized)
		if(check_validity(buffer,&num_words,words) == 0){
			//if it is, print the query back to the user
			print_words(num_words, words);
		
			//create an array of ordered lists, one per word
			ordered_list *lists[num_words];
			
			//avoid later segfault
			for(int i = 0; i<num_words; i++)
				lists[i] = NULL;
			
			//int array representing the status of each word
			//	runs parallel to the lists[] and words[] arrays
			//	0 means not operator; 1 means 'and'; 2 means 'or
			int *operators = malloc(sizeof(int) * num_words);
			
			//loop through each word in the query
			for(int i = 0; i<num_words; i++){
			
				//if the word isn't an operator
				if(!is_operator(words[i])){
				
					//tell our array that this word is not an operator
					operators[i] = 0;
					
					//get the counters with docID's and frequencies for our word
					counters = index_find(index, words[i]);
				
					//create an ordered list for the docID's from this word
					lists[i] = list_new(argv[1]);
						
					//if we found counters for that word
					if(counters!= NULL){
				
						//pass catalog_doc(void *arg, const int key, int count) 
						//	to counters_iterate to format output correctly
						//	function: fill the ordered_ list with score, frequency, url
						//	(see catalog_doc() for more information)
						counters_iterate(counters,catalog_doc,lists[i]);
					}
					
				//if it is an operator, set the correct number for our array
				} else
					operators[i] = !strcmp(words[i], "and") ? 1 : 2;
			}
			
			//create the master list from the array of lists
			ordered_list *master = create_master(argv[1],lists,operators,num_words);
			
			//print the list
			list_print(stdout, master);
			
			//cleanup
			free(operators);
			
			//cleanup the big list
			list_delete(master);
			
			//cleanup all the little lists
			for(int i = 0; i<num_words; i++)
				list_delete(lists[i]);

		}
		
		//free the memory afterwards
		free_words(num_words, words);
		
		printf("-------------------------------\nKeywords: ");
	}
	
	index_delete(index);
	return(0);
} 
 
/*	program to parse command line queries and tell
 *	the user if it's wrong
 * 	returns 1 upon failure, 0 upon success.
 *	assumptions:	buffer is of a length greater than 0
 */
static int check_validity(char *buffer, int *num_words, char **words)
{
	int i;
	char *tok;
	const char s[3] = " \t";
	
	//make sure there are actually words and not just spaces
	if(!contains_chars(buffer)){
		fprintf(stderr, "Queries must be words.\n");
		
		//set all to null so we don't accidentally free later
		for(i=0; i<strlen(buffer);i++)
			words[i] = NULL;
			
		return(1);
	}
	
	//use strtok to break up buffer into words
	tok = strtok(buffer, s);
	
	for(i = 0; tok != NULL; i++){
		words[i] = malloc(strlen(tok)+1);
		strcpy(words[i],tok);
		tok = strtok(NULL, s);
	}
	
	*num_words = i;
	
	//check if empty query: words can only have letters in them
	for(i=0;i<*num_words;i++){
	
		//normalize the word
		NormalizeWord(words[i]);
		
		//check each character
		for(int j=0;j<strlen(words[i]);j++){
			if(!isalpha(words[i][j])){
				fprintf(stderr, "Queries may only include alphabetical characters.\n");
				return 1;
			}
		}
	}
	
	//check if starts with an operator
	if(is_operator(words[0])){
		fprintf(stderr, "Queries may not begin with operators.\n");
		return 1;
	}
	
	//check if ends with an operator
	if(is_operator(words[*num_words-1])){
		fprintf(stderr, "Queries may not end with operators\n");
		return 1;
	}
	
 	//check if two consecutive operators
 	for(i=0;i<*num_words-1;i++){
 		if(is_operator(words[i]) && is_operator(words[i+1])){
 			fprintf(stderr, "Queries may not contain two consecutive operators\n");
 			return 1;
 		}
 	}
 	
 	return 0;
}

/*	function to pass to counters_iterate
 *	works with the counters_t returned form index_find to print
 *	the query results formatted correctly back to the user
 */
static void catalog_doc(void *arg, const int key, int count)
{
	//user passes in string path of pageDirectory containing docID's
	char filename[PATH_LEN];
	char url[URL_LEN];
	sprintf(filename, "%s/%d", ((ordered_list *)arg)->pageDirectory, key);
	
	//check if file exists and open it
	if( access( filename, F_OK ) != -1 ) {
    	FILE *fp = fopen(filename, "r");
		fgets(url, URL_LEN, fp);
		fclose(fp);
		
		//go to pageDirectory and open docID with the name of the key
		//scan the first line to get the url
		//printf("score %d doc %d: %s", count, key, url);
		list_insert((ordered_list*)arg, key, count, url);
	} else {
    	fprintf(stderr, "Error! File %s does not exist.\n", filename);
	}
}

/*	check if a word is one of the designated operators
 *	return true if it is, false if it is not
 *	assumptions: 	character array must end in '\0' and be a string
 *					word is normalized and only lowercase
 */
static bool is_operator(char *word)
{
	if(!strcmp(word, "and") || !strcmp(word, "or"))
		return true;
	else return false;

}

/*	check if a character buffer contains anything besides spaces
 *	returns true if yes; false if no
 */
static bool contains_chars(char *buffer)
{
	for(int i =0; i<strlen(buffer); i++){
		if(buffer[i] != ' ')
			return true;
	}
	
	return(false);
}

/*	function to free a 'words', which is an array of pointers to word strings
 *	split from a character buffer
 */
static void free_words(int length, char **words)
{
	for(int i=0; i<length; i++){
		if(words[i] != NULL)
			free(words[i]);
	}
	
	free(words);
}

/*	function to print a 'words', which is an array of pointers to word strings
 *	split from a character buffer.
 */
static void print_words(int length, char **words)
{
	printf("Query:");
	for(int i = 0; i<length; i++){
		printf(" %s", words[i]);
	}
	printf("\n");
}