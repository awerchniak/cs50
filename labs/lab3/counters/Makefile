# Makefile for 'counters' module
#
# Andy Werchniak, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = counterstest
OBJS = counterstest.o counters.o
LIBS =

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

bagtest.o: counters.h
bag.o: counters.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)