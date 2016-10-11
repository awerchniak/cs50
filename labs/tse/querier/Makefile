# Makefile for 'querier' module
#
# Andy Werchniak, August 2016

L = ../lib
C = ../common

PROG = querier
OBJS = querier.o $C/index.o
LIBS = 
LLIBS = $L/cs50ds.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# build the libraries
$L/cs50ds.a: 
	cd ../lib; $(MAKE)
$C/common.a:
	cd ../common; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd $L; $(MAKE) clean
	cd $C; $(MAKE) clean
