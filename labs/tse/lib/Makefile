# Makefile for 'cs50ds' library
#   Builds and compiles all the data structures into a library.
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016
# updated by Andy Werchniak, August 2016

OBJS = bag/bag.o counters/counters.o hashtable/hashtable.o hashtable/jhash.o set/set.o memory/memory.o
LIB = cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^

# add other targets for the other .o files
bag/bag.o: bag/bag.c bag/bag.h
	cd bag; $(MAKE) bag.o
counters/counters.o: counters/counters.c counters/counters.h
	cd counters; $(MAKE) counters.o
hashtable/hashtable.o: hashtable/hashtable.c hashtable/hashtable.h
	cd hashtable; $(MAKE) hashtable.o
hashtable/jhash.o: hashtable/jhash.c hashtable/jhash.h
set/set.o: set/set.c set/set.h
	cd set; $(MAKE) set.o
memory/memory.o: memory/memory.c memory/memory.h
	cd memory; $(MAKE) memory.o

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
	cd bag; $(MAKE) $@
	cd counters; $(MAKE) $@
	cd hashtable; $(MAKE) $@
	cd set; $(MAKE) $@
	cd memory; $(MAKE) $@