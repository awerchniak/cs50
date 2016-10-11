README for chill.c
Andy Werchniak, July 2016

Compiling:
	mygcc -o chill -lm chill.c

Usage:
/* 
 * chill.c - compute windchill from input arguments
 * 
 * usage: calculate windchill
 * where:
 *   targets are temperature and wind velocity values
 * output:
 *   calculate windchill from air temperature and wind velocity
 *   and output result in table to stdout
 * stdin: the input template
 * stdout: the modified template
 * stderr: error messages
 *
 * Andy Werchniak, July 2016
 */

Example command lines:

  ./chill
  ./chill 40
  ./chill 40 10

Exit status:
  0 - success
  1 - incorrect number of arguments
  2 - arguments out of bounds

Assumptions:

- arguments must be numerical values. Otherwise, they will be treated as the floating
  point value 0.0 (this is out of bounds for  windchill).

"Non"-Assumptions:

- inputs may be any floating point value or integer within range

Limitations:

- can only calculate windchill using the given formula:
	Wind Chill = 35.74 + 0.6215T - 35.75(V^0.16) + 0.4275T(V^0.16)
  (no other considerations)
