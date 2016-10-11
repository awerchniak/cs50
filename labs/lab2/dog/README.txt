README for dog.c
Andy Werchniak, July 2016

Compiling:
	mygcc -o dog dog.c

Usage:
/* 
 * dog.c - like cat but with different functionality
 * 
 * usage: echo contents of a file
 * where:
 *   target is a filename to read the contents of to stdout
 * output:
 *   contents of the file, with specified formatting options
 * stdin: the input template
 * stdout: the modified template
 * stderr: error messages
 *
 * My Name, July 2016
 */

Example command lines:

  dog -n testFile.txt - otherTestFile.txt
  dog -m mapfile.txt

Exit status:
  0 - success
  1 - Tags specified in invalid syntax
  2 - Error opening specified file
  3 - Invalid mapfile

Assumptions:
- Mapfile (for use in -m operation) must be formatted as follows:
	No more than 26 lines with no more than 9 characters per line
	No characters aside from letters, ' ', and '\n'
	Only two letters per line are allowed
- Encryption maintains case. For example, if a is mapped to Z in the mapfile,
	dog will simply map a to z and maintain the case of the input character.
	The same is true for decryption
- User must specify all tags BEFORE listing any file names for dog
- User may not specify the '--help' switch in addition to other switches
	(--help will override)

"Non"-Assumptions:
- User may specify any number of arguments
- The stdin '-' instruction may appear anywhere in the file sequence

Limitations:
- Does not contain full functionality of cat: only supports --help, -n, -m, and -M