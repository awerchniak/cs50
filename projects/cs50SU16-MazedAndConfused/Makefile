# Makefile for 'AMStartup' module
#
# Mazed and Confused, August 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb `pkg-config --cflags gtk+-3.0`
PROG = AMStartup
OBJS = AMStartup.o amazing_client.o amazing.o
LIBS = `pkg-config --libs gtk+-3.0` -pthread

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

AMStartup.o: amazing.h
amazing_client.o:
amazing.o:

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)
