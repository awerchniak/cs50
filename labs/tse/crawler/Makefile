# Makefile for 'crawler' module
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016
# updated by Andy Werchniak, August 2016

L = ../lib
C = ../common

PROG = crawler
OBJS = crawler.o
LIBS = -lcurl
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
