# Testing
### Test command line argument edge cases
```
[awerchniak@flume ]$ querier notADirectory ../data/indexFilename
pageDirectory does not exist.
[awerchniak@flume ]$ querier ../data/pageDirectory notAFile
indexFilename does not exist.
[awerchniak@flume ]$ mkdir testDir
[awerchniak@flume ]$ querier testDir ../data/indexFilename
Please entry a valid pageDirectory - the product of crawler.
```
### Test basic functionality
```
[awerchniak@flume ]$ querier ../data/pageDirectory ../data/indexFilename
Keywords: poop
Query: poop
No documents match.
-------------------------------
Keywords: Dartmouth College
Query: dartmouth college
score   4 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   3 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
-------------------------------
Keywords: dartmouth college
Query: dartmouth college
score   4 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   3 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
-------------------------------
Keywords: dartmouth and college
Query: dartmouth and college
score   4 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   3 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
-------------------------------
Keywords: 	dartmouth	AND	College
Query: dartmouth and college
score   4 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   3 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
-------------------------------

[awerchniak@flume ]$ leakVal querier ../data/pageDirectory ../data/indexFilename
==2731== Memcheck, a memory error detector
==2731== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==2731== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==2731== Command: querier ../data/pageDirectory ../data/indexFilename
==2731== 
Keywords: xia
Query: xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak
Query: werchniak
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
-------------------------------
Keywords: werchniak or xia
Query: werchniak or xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak and xia
Query: werchniak and xia
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
-------------------------------
Keywords: computer
Query: computer
score   4 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   3 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   3 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   2 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
-------------------------------
Keywords: science
Query: science
score   4 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: computer science
Query: computer science
score   2 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
-------------------------------
Keywords: werchniak and xia or computer science
Query: werchniak and xia or computer science
score   2 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
-------------------------------
Keywords: ==2731== 
==2731== HEAP SUMMARY:
==2731==     in use at exit: 0 bytes in 0 blocks
==2731==   total heap usage: 14,527 allocs, 14,527 frees, 461,570 bytes allocated
==2731== 
==2731== All heap blocks were freed -- no leaks are possible
==2731== 
==2731== For counts of detected and suppressed errors, rerun with: -v
==2731== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
### Test more... try and break it
```
[awerchniak@flume ]$ leakVal querier ../data/pageDirectory ../data/indexFilename
==13926== Memcheck, a memory error detector
==13926== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==13926== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==13926== Command: querier ../data/pageDirectory ../data/indexFilename
==13926== 
Keywords: xia
Query: xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak
Query: werchniak
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
-------------------------------
Keywords: werchniak or notAWord
Query: werchniak or notaword
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
-------------------------------
Keywords: xia or notAWord
Query: xia or notaword
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak or notAWord or xia
Query: werchniak or notaword or xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak or computer notaword or xia 
Query: werchniak or computer notaword or xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: computer notaword or werchniak or xia
Query: computer notaword or werchniak or xia
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak or xia or computer notaword
Query: werchniak or xia or computer notaword
score   3 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
-------------------------------
Keywords: werchniak or xia or computer notaword or science
Query: werchniak or xia or computer notaword or science
score   7 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   2 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   2 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
-------------------------------
Keywords: werchniak or xia or science
Query: werchniak or xia or science
score   7 doc    2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc    7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   2 doc   10: http://old-www.cs.dartmouth.edu/~cs50/
score   2 doc    1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   2 doc    4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc    9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
-------------------------------
Keywords: ==13926== 
==13926== HEAP SUMMARY:
==13926==     in use at exit: 0 bytes in 0 blocks
==13926==   total heap usage: 14,911 allocs, 14,911 frees, 750,702 bytes allocated
==13926== 
==13926== All heap blocks were freed -- no leaks are possible
==13926== 
==13926== For counts of detected and suppressed errors, rerun with: -v
==13926== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

### Test edge cases for query input
```
Keywords: [awerchniak@flume ]$ querier ../data/pageDirectory ../data/indexFilename
Keywords: and
Queries may not begin with operators.
-------------------------------
Keywords: or
Queries may not begin with operators.
-------------------------------
Keywords: and dartmouth
Queries may not begin with operators.
-------------------------------
Keywords: or dartmouth
Queries may not begin with operators.
-------------------------------
Keywords: dartmouth college or
Queries may not end with operators
-------------------------------
Keywords: dartmouth college and
Queries may not end with operators
-------------------------------
Keywords: dartmouth college and or computer
Queries may not contain two consecutive operators
-------------------------------
Keywords: dartmouth college and and computer
Queries may not contain two consecutive operators
-------------------------------
Keywords: dartmouth college or and computer
Queries may not contain two consecutive operators
-------------------------------
Keywords: computer science 50
Queries may only include alphabetical characters.
-------------------------------
Keywords: Dartmouth!
Queries may only include alphabetical characters.
-------------------------------
Keywords: Backus-Naur Form
Queries may only include alphabetical characters.
-------------------------------

[awerchniak@flume ]$ leakVal querier ../data/pageDirectory ../data/indexFilename
==2897== Memcheck, a memory error detector
==2897== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==2897== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==2897== Command: querier ../data/pageDirectory ../data/indexFilename
==2897== 
Keywords: 
Please enter a query with a length greater than 0.
-------------------------------
Keywords:                                       
Queries must be words.
-------------------------------
Keywords: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Query: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
No documents match.
-------------------------------
Keywords: Query: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
No documents match.
-------------------------------
Keywords: Query: aa
No documents match.
-------------------------------
Keywords: http://www.cs.dartmouth.edu/~cs50/Labs/Lab6/
Queries may only include alphabetical characters.
-------------------------------
Keywords: and werchniak
Queries may not begin with operators.
-------------------------------
Keywords: or werchniak
Queries may not begin with operators.
-------------------------------
Keywords: AND werchniak
Queries may not begin with operators.
-------------------------------
Keywords: OR werchniak
Queries may not begin with operators.
-------------------------------
Keywords: werchniak And
Queries may not end with operators
-------------------------------
Keywords: werchniak oR
Queries may not end with operators
-------------------------------
Keywords: werchniak and OR xia
Queries may not contain two consecutive operators
-------------------------------
Keywords: werchniak and xia or and davis
Queries may not contain two consecutive operators
-------------------------------
Keywords: ==2897== 
==2897== HEAP SUMMARY:
==2897==     in use at exit: 0 bytes in 0 blocks
==2897==   total heap usage: 14,299 allocs, 14,299 frees, 293,312 bytes allocated
==2897== 
==2897== All heap blocks were freed -- no leaks are possible
==2897== 
==2897== For counts of detected and suppressed errors, rerun with: -v
==2897== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
[awerchniak@flume ]$ 
```

### Driver for testing ordered_list data structure:
```
int main(void){
	
	//test list_new();
	ordered_list *list1 = list_new("pagedirectory");
	
	//test list_insert()
	list_insert(list1, 1, 10, "url1");
	list_insert(list1, 2, 3, "url2");
	list_insert(list1, 3, 11, "url3");
	list_insert(list1, 4, 11, "url4");
	list_insert(list1, 5, 10, "url5");
	list_insert(list1, 6, 3, "url6");
	list_insert(list1, 7, -4, "url7");
	
	//test list_print()
	printf("\nList 1:\n");
	list_print(stdout, list1);
	
	//test list_peek()
// 	hit_t *node = list_peek(list1, 6);
// 	printf("\nlist_peek():\n");
// 	if(node != NULL)
// 		printf("docID: %d frequency: %d url: %s\n", node->docID, node->frequency, node->url);
// 	else printf("(null)\n");
	
	//test list_append()	
	ordered_list *list2 = list_new("pagedirectory");
	
	list_insert(list2, 1, 10, "url1");
	list_insert(list2, 2, 7, "url2");
	list_insert(list2, 3, 4, "url3");
	list_insert(list2, 4, 11, "url4");
	
	printf("\nList2:\n");
	if(!list_print(stdout, list2))
		printf("empty list.\n");
	
	list_append(list2, list1);
	
	printf("\nAppended:\n");
	list_print(stdout, list2);
	
	ordered_list *list4 = list_new("pagedirectory");
	list_append(list4, list2);
	printf("\nAppended from scratch:\n");
	list_print(stdout,list4);
	
// 	
	//test list_intersect()
// 	ordered_list *list3 = list_intersect(list1, list2);
// 
// 	printf("\nIntersection:\n");
// 	list_print(stdout,list3);
	
	//test list_remove()
// 	list_remove(list1, 3);
// 	list_remove(list1, 4);
// 	printf("\nRemove 3,4:\n");
// 	list_print(stdout,list1);
// 	
// 	printf("\nTry to remove element not in the list:\n");
// 	list_remove(list1,17);
// 	
// 	list_remove(list1,5);
// 	list_remove(list1,7);
// 	printf("\nRemove 5,7:\n");
// 	list_print(stdout,list1);
// 	
// 	list_remove(list1,2);
// 	printf("\nRemove 2:\n");
// 	list_print(stdout,list1);
// 	
// 	list_remove(list1,1);
// 	list_remove(list1,6);
// 	printf("\nRemoved 1,6\n");
// 	if(!list_print(stdout,list1))
// 		printf("empty list.\n");
	
	//cleanup
	list_delete(list4);
// 	list_delete(list3);
	list_delete(list2);
	list_delete(list1);
	
	return(0);
}
```