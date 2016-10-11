README for weather.c
Andy Werchniak, July 2016

Compiling:
	mygcc -o weather weather.c -lcurl mycurl.c

Usage:
/* 
 * weather.c - get weather info from NOAA website
 * 
 * usage: web crawler for weather information
 * where:
 *   target is 4 letter weather code
 * output:
 *   reads various details about weather in a given region to stdout
 * stdin: the input template
 * stdout: the modified template
 * stderr: error messages
 *
 * Andy Werchniak, July 2016
 */

Example command lines:

  ./weather KMWN

Exit status:
  0 - success
  1 - invalid location code/number of arguments
  2 - error with curl

Assumptions:
- URL provided is correct link for NOAA website

"Non"-Assumptions:
- User may enter any valid 4 letter station code

Limitations:
- Only search one weather site (NOAA), no other data
