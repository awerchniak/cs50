# Testing for index.c

## use indextest.sh to verify that index_load() and index_save work together seamlessly
```
[awerchniak@flume ]$ ls pageDirectory
1  2  3  4  5  6  7
[awerchniak@flume ]$ indextest.sh pageDirectory
cleaned old source files
made index and indextest
ran indexer
first 3 lines to verify:
yor 7 1 
uyghurche 7 1 
rkmen 7 1 
ran indextest
first 3 lines to verify:
website 7 1 6 2 5 1 4 1 1 3 
offline 1 1 
locally 1 1 
sorting and comparing files
if no output, we passed the test!
cleaned up
```
Important: because there was no output from diff (between the 3rd to last and 2nd to
last lines), we know that the two functions work together!

## use valgrind to test indexer for memory leaks!
```
[awerchniak@flume ]$ leakVal indexer ../data/pageDirectory oldIndex
==10153== Memcheck, a memory error detector
==10153== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==10153== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==10153== Command: indexer ../data/pageDirectory oldIndex
==10153== 
==10153== 
==10153== HEAP SUMMARY:
==10153==     in use at exit: 0 bytes in 0 blocks
==10153==   total heap usage: 1,545,900 allocs, 1,545,900 frees, 101,477,268,501 bytes allocated
==10153== 
==10153== All heap blocks were freed -- no leaks are possible
==10153== 
==10153== For counts of detected and suppressed errors, rerun with: -v
==10153== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## use valgrind and indextest to test index_load() for memory leaks
```
[awerchniak@flume ]$ leakVal indextest oldIndex newIndex
==11055== Memcheck, a memory error detector
==11055== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==11055== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==11055== Command: indextest oldIndex newIndex
==11055== 
==11055== 
==11055== HEAP SUMMARY:
==11055==     in use at exit: 0 bytes in 0 blocks
==11055==   total heap usage: 138,829 allocs, 138,829 frees, 3,237,995 bytes allocated
==11055== 
==11055== All heap blocks were freed -- no leaks are possible
==11055== 
==11055== For counts of detected and suppressed errors, rerun with: -v
==11055== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## test edge case: nonexistent directory to indexer
```
[awerchniak@flume ]$ indexer notADirectory newFile
Please enter a valid pageDirectory.
```

## test edge case: nonexistent old file to indextest
```
[awerchniak@flume ]$ indextest notAFile newFile
Please enter a valid oldIndexFilename.
```

## test invalid number of inputs
```
awerchniak@flume ]$ indexer first second third
Error! Indexer takes two inputs: pageDirectory and fileName
[awerchniak@flume ]$ indexer
Error! Indexer takes two inputs: pageDirectory and fileName
[awerchniak@flume ]$ indexer first
Error! Indexer takes two inputs: pageDirectory and fileName
[awerchniak@flume ]$ indextest first
Error! Indexer takes two inputs: oldIndexFilename and newIndexFilename
[awerchniak@flume ]$ indextest first second third
Error! Indexer takes two inputs: oldIndexFilename and newIndexFilename
[awerchniak@flume ]$ indextest
Error! Indexer takes two inputs: oldIndexFilename and newIndexFilename
```

## ensure that the outputs are formatted correctly using cat
```
[awerchniak@flume ]$ indexer pageDirectory old
[awerchniak@flume ]$ head -10 old
yor 7 1 
uyghurche 7 1 
rkmen 7 1 
furlan 7 1 
emili 7 1 
technosignature 7 1 
integration 7 1 
technoscience 7 1 
technocracy 7 1 
neolithic 7 1 
[awerchniak@flume ]$ indextest old new
[awerchniak@flume ]$ head -10 new
website 7 1 6 2 5 1 4 1 1 3 
offline 1 1 
locally 1 1 
wikipedia 7 13 6 12 5 11 4 11 3 11 2 13 1 6 
and 7 225 6 202 5 163 4 287 3 121 2 137 1 1 
contributors 1 1 
leto 1 1 
arab 7 1 6 1 5 1 4 1 3 1 2 1 
wgcanonicalspecialpagename 7 1 6 1 5 1 4 1 3 1 2 1 
wgarticleid 7 1 6 1 5 1 4 1 3 1 2 1 
```
note: many of these are not words. That is OK, because it is beyond the scope of
this assignment to verify if character strings encountered within HTML are actually
real words

## use 'ls' to ensure make works correctly
```
[awerchniak@flume ]$ ls
DESIGN.md  indextest.c   Makefile       README.md
indexer.c  indextest.sh  pageDirectory  TESTING.md
[awerchniak@flume ]$ make
gcc -Wall -pedantic -std=c11 -ggdb -I../lib -I../common   -c -o indexer.o indexer.c
gcc -Wall -pedantic -std=c11 -ggdb -I../lib -I../common   -c -o ../common/index.o ../common/index.c
cd ../common; make
make[1]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/common'
gcc -Wall -pedantic -std=c11 -ggdb -I../lib   -c -o file.o file.c
gcc -Wall -pedantic -std=c11 -ggdb -I../lib   -c -o web.o web.c
gcc -Wall -pedantic -std=c11 -ggdb -I../lib   -c -o webpage.o webpage.c
gcc -Wall -pedantic -std=c11 -ggdb -I../lib   -c -o word.o word.c
ar cr common.a file.o web.o index.o webpage.o word.o
make[1]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/common'
cd ../lib; make
make[1]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib'
cd bag; make bag.o
make[2]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/bag'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o bag.o bag.c
make[2]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/bag'
cd counters; make counters.o
make[2]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/counters'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o counters.o counters.c
make[2]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/counters'
cd hashtable; make hashtable.o
make[2]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/hashtable'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o hashtable.o hashtable.c
make[2]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/hashtable'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o hashtable/jhash.o hashtable/jhash.c
cd set; make set.o
make[2]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/set'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o set.o set.c
make[2]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/set'
cd memory; make memory.o
make[2]: Entering directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/memory'
gcc -Wall -pedantic -std=c11 -ggdb   -c -o memory.o memory.c
make[2]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib/memory'
ar cr cs50ds.a bag/bag.o counters/counters.o hashtable/hashtable.o hashtable/jhash.o set/set.o memory/memory.o
make[1]: Leaving directory '/net/tahoe3/awerchniak/cs50/labs/tse/lib'
gcc -Wall -pedantic -std=c11 -ggdb -I../lib -I../common indexer.o ../common/index.o ../common/common.a ../lib/cs50ds.a  -o indexer
gcc -Wall -pedantic -std=c11 -ggdb -I../lib -I../common   -c -o indextest.o indextest.c
gcc -Wall -pedantic -std=c11 -ggdb -I../lib -I../common indextest.o ../common/index.o ../common/common.a ../lib/cs50ds.a  -o indextest
[awerchniak@flume ]$ ls
DESIGN.md  indexer.c  indextest    indextest.o   Makefile       README.md
indexer    indexer.o  indextest.c  indextest.sh  pageDirectory  TESTING.md
```

## below is a short test program to verify that my read_ints function works:
```
int main(void){
	
	char *string = "key 5.17 12\n3~~x4 587653 +6 12 16 -4";
	int first, second, pos;
	
	pos = strlen(string);

	while(read_ints(string,&pos,&first,&second)>0)
		printf("%d %d\n", first, second);
	
	return(0);
}

Output:
andywersmacbook:desktop andywerchniak$ ./test
16 4
6 12
4 587653
12 3
5 17
```
Note: read_ints intentionally reads pairs from the back to the front, because 
counters_print prints from the beginning to the end, but counters_insert inserts 
from the back to the front.
