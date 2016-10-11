# Makefile for TSE
#
# Andy Werchniak, August 2016

MAKE = make

all:
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier
	$(MAKE) -C lib
	$(MAKE) -C common

.PHONY: clean
	
clean:
	cd crawler; $(MAKE) clean
	cd indexer; $(MAKE) clean
	cd querier; $(MAKE) clean
	cd lib; $(MAKE) clean
	cd common; $(MAKE) clean
