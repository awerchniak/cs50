/* dog.c
 * 
 * This program works like the 'cat' command in Unix (see documentation), but has different
 * tag options. It supports -n (numbered lines), -m (mapfile encryption), and -M (mapfile
 * decryption).
 *
 * Created by Andy Werchniak, July 2016
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

/* Preprocessor directives */
#define NUMCHARS 52												//allocate double the alphabet for safety's sake
#define MAX_LINE 10												//max length for line in mapfile

/* global variables */
static int lines_to_print=0;									//line number for -n
static char left[NUMCHARS];										//parallel arrays for encrypting/decrypting
static char right[NUMCHARS];

void printHelp(void);
void cat(const char *arg, int choice);
void printLines(const char *arg, int choice);
bool isValidMapfile(const char *mapfile);
void makefileArrays(const char *mapfile);
char mapChar(char c, int choice);


/* Main : takes arguments and decides which function variations to use
 * Parameters: argc (# of arguments), argv(array of arguments)
 * return: 0 if all goes well (README.txt)
 */
int main(const int argc, const char *argv[])
{
	/* Declare variables */
	bool n = false, m = false, M = false;
	char c;
	int i,firstFileIndex;
	const char *mapfile;										//character pointer that will point to argv[] element 
																// storing the mapfile name. For convenience
	
	/* Check the number of arguments */
	switch(argc){
		case 0: //this should never happen....
			printf("How did you manage to do this? That's impossible!\n");
			exit(1);
			break;
		case 1: //no arguments: repeat stdin
			while(fscanf(stdin, "%c", &c)!=EOF)
				printf("%c", c);
			break;
		case 2: //one argument means either get help, number stdin, or send file contents to stdout
			if(strcmp(argv[1],"--help") == 0)
				printHelp();
			else if(strcmp(argv[1],"-n") == 0)
				printLines("-",0);
			else {
				cat(argv[1], 0);
			}
			break;
		default:
			//scan in all of the commands
			//start at argv[1] b/c argv[0] will just be "dog"
			for(i=1;i<argc;i++){	
				
				//if any of them are "--help", print out the help info and end
				if(strcmp(argv[i], "--help")==0){
					printHelp();
					return(0);
				}
				
				//if any are "-n", we will number lines
				if(strcmp(argv[i],"-n")==0){
					n = true;
				}
				
				//if any are "-m", we will mapfile encrypt
				if(strcmp(argv[i],"-m")==0){
					if(M == false){								//make sure -m and -M don't appear together
						if(!(i+1 == argc)){						//avoid segfault: no mapfile specified
							m = true;
							mapfile = argv[++i];				//set the mapfile pointer
						}
					} else{
						printf("Error! Please enter only -m or -M, but not both.\n");
						exit(1);
					}
				}
				
				//if any are "-M", we will mapfile decrypt
				if(strcmp(argv[i],"-M")==0){
					if(m == false){								//make sure -M and -m don't both appear
						if((i+1 == argc)){						//avoid segfault: no mapfile specified
							M = true;
							mapfile = argv[++i];				//set the mapfile pointer
						}
					} else{
						printf("Error! Please enter only -m or -M, but not both.\n");
						exit(1);
					}
				}
			}
			
			//now let the dogs out
			if(n){
				if(m){											//print all files encrypted with numbered lines
					firstFileIndex = 4;							//first file is argv[4]
					makefileArrays(mapfile);					//create parallel arrays using mapfile
					if(firstFileIndex == argc){					//if no file, do from stdin
						printLines("-",1);
					} else{										//otherwise, printLines all files
						for(i=firstFileIndex;i<argc;i++)
							printLines(argv[i],1);
					}
				} else if (M){									//print all files decrypted with numbered lines
					firstFileIndex = 4;							//first file is argv[4]
					makefileArrays(mapfile);					//create parallel arrays using mapfile
					if(firstFileIndex == argc)					//if no file, do from stdin
						printLines("-",2);
					else{										//otherwise, printLines all files
						for(i=firstFileIndex;i<argc;i++)
							printLines(argv[i],2);
					}
				} else{											//print all files with numbered lines
					firstFileIndex = 2;							//first file is argv[2]
					for(i=firstFileIndex;i<argc;i++)
						printLines(argv[i],0);
				}
			} else if (m){										//print all files encrypted from mapfile
				firstFileIndex = 3;								//first file is argv[3]
				makefileArrays(mapfile);						//create parallel mapping arrays
				if(firstFileIndex == argc){						//if no file, do from stdin
					cat("-",1);
				} else{
					for(i=firstFileIndex;i<argc;i++)			//otherwise, print all files
						cat(argv[i],1);
				}
			} else if (M){										//print all files decrypted from mapfile
				firstFileIndex = 3;								//first file is argv[3]
				makefileArrays(mapfile);						//create parallel mapping arrays
				if(firstFileIndex == argc){						//if no file, do from stdin
					cat("-",2);
				} else{											//otherwise, print all fies
					for(i=firstFileIndex;i<argc;i++)
						cat(argv[i],2);
				}
			} else{												//this shouldn't happen
				firstFileIndex = 1;								//but, just in case, print all files
				for(i=firstFileIndex;i<argc;i++)
					cat(argv[i],0);
			}
	}

	/* If we got this far, all is well so return 0*/
	return(0);
}

/*printHelp : print the help message
 */
void printHelp(void)
{
	printf("Usage: dog [OPTION]... [FILE]...\n");
	printf("Like cat, but slightly different!\n\n");
	printf("With no FILE, or when FILE is -, read standard input.\n\n");
	printf("\t-n\t\t\tnumber lines on output\n");
	printf("\t-m\t\t\tweakly encrypt file using specified mapfile\n");
	printf("\t\t\t\tcannot be used in conjuction with -M\n");
	printf("\t-M\t\t\tweakly decrypt file using specified mapfile\n");
	printf("\t\t\t\tcannot be used in conjuction with -m\n");
	printf("      --help     display this help and exit\n\n");
	printf("Examples:\n");
	printf("dog -m mapfile.txt encrypt standard output using map in mapfile.txt\n");
	printf("dog -M mapfile.txt encryption.txt decrypt 'encryption.txt' using mapfile.txt\n");
	printf("dog -n repeat stdin with numbered lines\n");
	printf("dog -n -m mapfile.txt text.txt number lines, encrypt 'test.txt'\n");
	printf("dog	copy stdin to stdout\n\n");
	printf("Information on valid mapfiles available in README.txt\n");
	
}

/* cat : like a normal cat function. Also supports -m/M mapping
 * parameters: arg (name of file to write to stdout), "-" specifies stdin as file
 * 			   choice (0 for nothing, 1 for encrypt, 2 for decrypt)
 */
void cat(const char *arg, int choice)
{
	/* declare local variables */
	char c;
	FILE *fp;
	
	if(access(arg, F_OK) != -1 || strcmp(arg, "-") == 0){			//make sure dog can open the file
		if(strcmp(arg, "-"))										// if the file name is "-", get stdin
			fp = fopen(arg, "r");									// otherwise, open the file
		else fp = stdin;
	
		while(fscanf(fp,"%c",&c)!=EOF){								//scan until the end of the file
			if(!choice)												//if not encrypting, just print
				printf("%c", c);
			else printf("%c", mapChar(c,choice));					//otherwise, print encrypted/decrypted
		}
	
		if(fp != stdin && fp != NULL)								//avoid double free-ing bug
			fclose(fp);												//close the file pointer
		else fp = NULL;
	} else{
	 	printf("dog: %s: No such file or directory\n", arg);
	 	exit(2);
	}
}

/* printLines : line numbered version of cat. Also supports -m/M mapping
 * parameters: arg (name of file to write to stdout), choice (0 for nothing, 1 for -m, 2 for -M)
 */
void printLines(const char *arg, int choice)
{
	FILE *fp;
	char c[2] = "\0";												//declare c as a 1 character string so we can use string.h functions
	char line[1000];												//lines can be up to 999 characters long
	
	if(access(arg, F_OK) != -1 || strcmp(arg, "-") == 0){			//only open if file exists
		if(strcmp(arg, "-"))										//"-" specifies fp as stdin
			fp = fopen(arg, "r");									//otherwise, open file!
		else fp = stdin;
	
		for(; fscanf(fp,"%c",c)!=EOF; lines_to_print++){			//scan until end of file, increment line #
			if(choice) 												//if we are encryping/decrypting, do so
				*c = mapChar(*c, choice);
			strcpy(line,c);											//add character to the line so that it isn't lost during test
			while(fscanf(fp,"%c",c)!=EOF && c[0]!='\n'){			//scan until end of file or new line is encountered
				if(choice)											//if encrypting/decrypting, do so
					*c = mapChar(*c,choice);
				strcat(line,c);										//add each character to the line
			}
			printf("%d. %s\n",lines_to_print+1,line);				//print the whole line
		}
		
		if(fp != stdin && fp != NULL)								//avoid double free-ing bug
			fclose(fp);												//close the file pointer
		else fp = NULL;
	} else{
	 	printf("dog: %s: No such file or directory\n", arg);
	 	exit(2);
	}
}

/* isValidMapfile : tests whether a mapfile is valid. Exits if not
 *
 * A valid map file consists of 26 letters lines, only ' ', '\n', and letters, and no repeated mappings
 * if there are more than 9 characters per line, the remainder will be ignored
 * the first two letters that occur are mapped together
 *
 * Parameters: mapfile (file to test)
 * returns: true if the mapfile is ok
 */
bool isValidMapfile(const char* mapfile)
{
	/* Declare variables */
	FILE *fp;
	char c;
	char line[MAX_LINE];										//lines can be up to 9 characters long
	int i,letterCount;
	
	if(access(mapfile, F_OK) != -1 ){							//test if we can open the file
		fp = fopen(mapfile, "r");								//if so, open
	
		for(i=0; fscanf(fp,"%c",&c)!=EOF; i++){					//scan until end of file
			letterCount = 0;
																//if we encounter something that isn't a letter, ' ', or '\n', quit
			if(!((c>64 && c<91) || (c>96 && c<123) || c== ' ' || c == '\n')){
				printf("Error! Invalid mapfile : invalid characters. See documentation.\n");
				exit(3);
			} else if(c != ' ' && c != '\n'){
				letterCount++;									//otherwise if it's a letter, increment our letter index
			}
			while(fscanf(fp,"%c",&c)!=EOF && c!='\n' && strlen(line)<MAX_LINE-1){	//scan until end of file or new line is encountered
																//if we encounter something that isn't a letter, ' ', or '\n', quit
				if(!((c>64 && c<91) || (c>96 && c<123) || c== ' ' || c == '\n')){
					printf("Error! Invalid mapfile : invalid character '%c.' See documentation.\n", c);
					exit(3);
				} else if(c != ' ' && c != '\n'){
					letterCount++;								//otherwise if it's a letter, increment our letter index
				}
				if(letterCount > 2){							//make sure there are only two letters per line
					printf("Error! Invalid mapfile : too many letters per line. See documentation\n");
					exit(3);
				}
			}
			
		}
		
		if(i>26){												//ensure there are only 26 lines
			printf("Error! Invalid mapfile : invalid length %d. See documentation.\n", i);
			exit(3);
		}

		fclose(fp);
	} else{
	 	printf("dog: %s: No such file or directory\n", mapfile);
	 	exit(2);
	}
	
	return true;
}

/* makefileArrays : fill the left[] and right[] arrays from a mapfile for encrypting/decrypting
 *
 * parameters: mapfile (file to fill arrays from)
 */
void makefileArrays(const char *mapfile)
{
	/* Declare variables*/
	FILE *fp;
	char c;
	int j, i=0, leftCount=0, rightCount=0;
	
	if(!isValidMapfile(mapfile)){								//check if the mapfile is valid
		printf("We shouldn't get here....\n");
		exit(3);
	}
	
	if(access(mapfile, F_OK) != -1){							//check if file can be opened
		fp = fopen(mapfile, "r");								//if so, open it
		
		while(fscanf(fp, "%c",&c)!=EOF){						//scan until end of file
			if(c!=' ' && c!='\n'){
				if(i++%2==0){									//increment i. if %2=0, fill left. Else fill right
					/* check if left already contains c */
					for(j=0; j<leftCount; j++)					//iterate through left
						if(toupper(left[j]) == toupper(c)){		//if it's there, spit out an error and quit
							printf("Error! Invalid mapfile : repeated characters. See documentation.\n");
							exit(3);							
						}
					left[leftCount++]=c;						//otherwise, increment left index and add c to the array
				}
				else{
					/* check if right already contains c */
					for(j=0; j<rightCount; j++)					//iterate through right
						if(toupper(right[j]) == toupper(c)){	//if character already there, spit out error and quit
							printf("Error! Invalid mapfile : repeated characters. See documentation.\n");
							exit(3);
						}
					right[rightCount++]=c;						//otherwise, increment right index and add c to the array
				}
			}
		}
	
		fclose(fp);												//close the file
	} else {
		printf("Error! The mapfile %s could not be opened.\n", mapfile);
		exit(2);
	}
}

/* mapChar : encrypt or decrypt a character based on left[] and right[] parallel arrays
 * arguments: c (character to encrypt) choice (1 means encrypt, 2 means decrypt)
 * returns: encrypted/decrypted character
 */
char mapChar(char c, int choice)
{
	/* Declare variables*/
	int i;
	bool found = false;
	
	if(isgraph(c)){												//only encrypt visible characters
		for(i=0; !found && i<NUMCHARS;i++){						//iterate through left[] or right[] until found
			if(choice == 1 && toupper(left[i])==toupper(c)){	//if we're encrypting and it matches left[], that's it!
				found = true;
				/* Return a letter of the same case */
				if((left[i] > 96 && c > 96) || (left[i]<97 && c<97))
					return(right[i]);							//if same case, just return
				else if(left[i] > 96 && c<97)
					return((char)(right[i] - 32));				//if map lower and c upper, return upper map
				else return((char)(right[i] + 32));				//if map upper and c lower, return lower map
			} else if(choice ==2 && toupper(right[i])==toupper(c)){//if decrypting and it matching right[], that's it!
				found = true;
				/* Return a letter of the same case */
				if((right[i] > 96 && c > 96) || (right[i]<97 && c<97))
					return(left[i]);							//if same case, just return
				else if(right[i] > 96 && c<97)
					return((char)(left[i] - 32));				//if map lower and c upper, return upper map
				else return((char)(left[i] + 32));				//if map upper and c lower, return lower map
			}
		}
		//fprintf(stderr, "Warning: Char '%c' not included in map file.\n", c);
		return(c); 												//return encrypted character
	} else return(c);											//dont encrypt spaces, new lines, etc.
}