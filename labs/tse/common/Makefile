# Makefile for 'common.a' library
#   Builds and compiles all the files into a library.
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016
# updated by Andy Werchniak, August 2016

L = ../lib

OBJS = file.o web.o index.o webpage.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^
	
# source dependencies:
file.o: file.h
web.o: web.h
index.o: index.h file.h $L/hashtable/hashtable.h $L/counters/counters.h
webpage.o: webpage.h file.h $L/memory/memory.h
word.o: word.h

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
	cd ../lib; $(MAKE) clean