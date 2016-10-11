# Makefile for indexer.c
#
# Andy Werchniak, August 2016

L = ../lib
C = ../common

PROGS = indexer indextest
LIBS = 
LLIBS = $C/common.a $L/cs50ds.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make

all: $(PROGS)

# build the indexer
indexer: indexer.o $C/index.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	
# build indextest
indextest: indextest.o $C/index.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	
# build the libraries
$L/cs50ds.a: 
	cd $L; $(MAKE)
$C/common.a:
	cd $C; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROGS)
	cd ../lib; $(MAKE) clean
	cd ../common; $(MAKE) clean
