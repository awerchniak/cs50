# Makefile for 'set' module
#
# Andy Werchniak, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = settest
OBJS = settest.o set.o
LIBS =

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

treetest.o: set.h
tree.o: set.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)