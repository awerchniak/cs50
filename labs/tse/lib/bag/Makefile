# Makefile for 'bag' module
#
# Andy Werchniak, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = bagtest
OBJS = bagtest.o bag.o
LIBS =

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

bagtest.o: bag.h
bag.o: bag.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)