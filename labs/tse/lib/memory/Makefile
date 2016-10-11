# Makefile for 'memory' module
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016

# PROG =
OBJS = memory.o
LIBS =

MAKE = make
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

# I have no tester program
# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

memory.o: memory.h

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
