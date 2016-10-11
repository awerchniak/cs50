# Tiny Search Engine

## Functionality
> This is a tiny search engine! it parses web html to find sites with a user's keywords,
> and then returns the webpages to the user in which those keywords appear most frequently.
>
> Tiny Search Engine is composed of three major components: the crawler, the indexer,
> and querier. See /crawler/README.md, /indexer/README.md, and /querier/README.md for
> in depth explanations.

## Compiling
      make
## cleanup
      make clean

## Exit codes
      0.	No problems- programs run smoothly
      1.	Invalid arguments passed from command line
      99.	Error with malloc

## Assumptions
      1. Due to the limitations of crawler.c, the TSE only searches internal URLs,
         meaning those that begin with "http://old-www.cs.dartmouth.edu/" and that
         are written in HTML.
	  2. Due to the limitations querier.c, users' queries may only contain alphabetical
	  	 characters and should be fewer than 1000 characters long. pageDirectory paths
	  	 must also be less than 100 characters long.
      3. See /crawler/README.md, /indexer/README.md, and /querier/README.md for information
      	 on the individual programs.

## Non-Assumptions
      1. The user may specify any keywords of any length
      2. The user may pass any search depth to crawler, although depths above 2 are 
         not advised (see crawler documentation).
      3. See /crawler/README.md, /indexer/README.md, and /querier/README.md for information
      	 on the individual programs.

## Limitations
      1. Websites do not like crawlers, so although the user may pass any depth,
         any depth over 2 will take a very long time (as the crawler must wait
         1 second between visiting separate pages).
      2. See /crawler/README.md, /indexer/README.md, and /querier/README.md for information
      	 on the individual programs.