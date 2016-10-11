# Crawler.c

## Functionality
> Given a URL, parses the html to find linked webpages. Parses those until 
> it reaches a specified depth, and outputs the contents of each page to
> files in a specified directory.

## Compiling
      make
## cleanup
      make clean

## Usage example
      crawler seedURL pageDirectory maxDepth
      crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./test 1

## Exit codes
      0.	No problems- program runs smoothly
      1.	Invalid parameters passed from command line
      99.	Error with malloc

## Assumptions
      1. 	Crawler only works with valid, internal URLs. For this code,
      		these begin with "http://old-www.cs.dartmouth.edu/" and are written
      		in HTML
      2.	The user may only pass in a valid, writable directory for the 
      		pageDirectory argument (crawler will not create a new directory).
      3.	pageDirectory should not have any files in it with numeric names.
      		Crawler will not overwrite these files, so if they exist, crawler
      		will not effectively write its output.

## Non-Assumptions
      1.	maxDepth may be any positive integer. However, the program's
      		runtime increases exponentially as maxDepth increases.

## Limitations
      1.	crawler only works with internal URLS (see assumptions)
      2.	heap memory of the machine it runs on!
