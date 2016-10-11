/*
* AMStartup.c     		MazedAndConfused     		August, 2016
*
* This program is designed to solve a maze provided by the Dartmouth CS servers
* using the Left-Hand Rule. As it tries to solve to the maze it will print out
* the walls and bounderies it finds to the console with ASCII graphics and using
* GTK, a GUI that helps you vizualize the maze solving process more efficiently.
*
* USAGE: AMStartup [-n nAvatars] [-d difficulty] [-h hostname]
*
* It includes functions:
*
*	main: Calls appropriate helper functions based on options. Calling it sets up
*					the datastrctures and creates all of the threads.
*
*		input: int argc, the number of arguments passed into the function
*			   	 char *argv[], a pointer to the array of the argument values
*
*		return: int, 0 if exit successfully, non-zero if there is an error
*
*
*	create_info: creates a malloc'd struct with all of the maze info in it to
* 						 pass to each individual avatar thread
*
*		input: int AvatarID, the ID of the Avatar
*				 	 uint32_t nAvatars, the number of avatars
*				 	 uint32_t Difficulty, the difficulty of the maze
*				 	 char* hostname, the server hostname
*		 		 	 uint32_t MazePort, the port on which the maze is run
*				 	 char* logname, the filename for the logfile
*					 uint32_t MazeWidth, the width of the maze
*				 	 unint32_t MazeHeight, the height of the maze
*
*		return: thread_info, the struct with all of the data in it
*
*
*	delete_info: frees the memory malloc'd by create_info
*
*		input: thread_info *info, a pointer to the struct to free
*
*		return: (void)
*
*
*/

/*********************************** IMPORTS **********************************/
#include <stdlib.h>					//getenv(), malloc()
#include <unistd.h>					//getopt
#include <getopt.h>					//getopt_long
#include <stdio.h>					//printf, etc.
#include <stdbool.h>				//bool type
#include <netdb.h>					//socket-related structures
#include <string.h>					//memcpy, memset
#include <stdint.h>					//unint32_t, etc.
#include <errno.h>					//strerror
#include <time.h>					//time functions
#include <pthread.h>				//threads!
#include <gtk/gtk.h>				//gtk

/******************************** LOCAL IMPORTS *******************************/
#include "amazing.h"
#include "amazing_client.h"

/************************* GLOBAL VARIABLE DEFINITIONS ************************/
maze_t *maze;
FILE * logfile;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

/* ******************************************************************
* Name: main
*
* Description: The main function in this program. Calls appropriate
* 	helper functions based on options. Calling it sets up the datastrctures
*		and creates all of the threads.
*
* Input: int argc, the number of arguments passed into the function.
*			Must be a number between 1 and 3, which is determined
*			automatically based on the number of arguments passed in
*			to script.
*		 char *argv[], a pointer to the array of the argument values.
*			Created automatically based in arguments passed in to
*			script.
*
* Return: 			 Type: Int
*		  					 Possible Values: 0 – Exited without error
*																	Non-Zero – Exited with error
*
*/
int main(int argc, char *argv[])
{
	// Init local variables
	char c;
	AM_Message init_message = {.type = htonl(AM_INIT), .init.nAvatars = htonl(2),
		.init.Difficulty = htonl(0)};
	char *hostname = "flume.cs.dartmouth.edu";
	uint32_t MazePort, MazeWidth, MazeHeight;


	//validate command line parameters, based largely on Robert Sayegh's dog.c
	while ((c = getopt_long(argc, argv, "n:d:h:", NULL, NULL)) != -1) {
		switch(c) {
			// if user specifies the number of avatars
			case 'n':
				// validate that it is a valid number of avatars or return exit status 1
				if(atoi(optarg) < 2 || atoi(optarg) > 10){
					fprintf(stderr, "Please enter an nAvatars 2-10.\n");
					return(1);
				}
				// set the number of avatars in the init message
				init_message.init.nAvatars = htonl(atoi(optarg));

				// Break from the switch statement
				break;

			// if the user specifies the difficulty
			case 'd':
				// verifiy the validity of the difficulty or return exit status 1
				if(atoi(optarg) > 9 || atoi(optarg) < 0){
					fprintf(stderr,"Please enter a difficulty 0-9.\n");
					return(1);
				}

				// set the number of avatars in the init message
				init_message.init.Difficulty = htonl(atoi(optarg));

				// break from the switch statement
				break;

			// If the user specifies a hostname
			case 'h':
				// set the hostname to the input
				hostname = optarg;

				// Break from the switch statement
				break;

			// If the option specified is unknown
			case '?':
				// print the usage information and return an exit status 1
				fprintf(stderr, "Usage: AMStartup [-n nAvatars] [-d difficulty]\
					[-h hostname]\n");
				return(1);

				// Break from the switch statement (unreachable code, but good form)
				break;

			// If none of the above, treat as unknown (should never do this)
   		default:
				// print the usage information and return exit status 1
   			fprintf(stderr, "Usage: AMStartup [-n nAvatars] [-d difficulty]\
					[-h hostname]\n");
     		return(1);

				// Break from the switch statement (unreachable code, but good form)
     		break;
		}
	}

	// display relevant information about the maze to the user
	printf("nAvatars: %d, Difficulty: %d, hostname: %s\n",
		ntohl(init_message.init.nAvatars), ntohl(init_message.init.Difficulty),
		hostname);

	// Create socket to connect
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	// Make sure we actually created a socket or error out
	if (sock < 0){
		perror("opening socket");
		return(2);
	}

	// Initialize the fields of the server address
	struct sockaddr_in server = {.sin_family = AF_INET, .sin_port =
		htons(atoi(AM_SERVER_PORT))};
	struct hostent *hostp;

	// Get the hostnmae
	hostp = gethostbyname(hostname);

	// Make sure we could get the hostname or error out with exit status 3
	if (hostp == NULL){
		fprintf(stderr, "AMStartup: unknown host '%s'\n", hostname);
		return(3);
	}

	// Initialize the memory for sin_addr
	memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

 	// Connect the socket to that server or error out with exit status 4
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		perror("connecting stream socket");
		return(4);
	}

	// Send AM_INIT message to server or error out with exit status 5
	if (write(sock, &init_message, sizeof(AM_Message)) < 0){
		perror("writing on stream socket");
      	return(5);
	}

	// Receive a message back from the server
	AM_Message return_message;

	// Make sure we got a message or return exit status 6
	if(read(sock, &return_message, sizeof(AM_Message)) < 0){
		perror("reading from stream socket");
		return(6);
	}

	// Check if that message is AM_INIT_OK, AM_INIT_FAILED, or something else
	switch(ntohl(return_message.type)){
		// If server says all good, set the appropriate varaibles
		case AM_INIT_OK:
			MazePort = ntohl(return_message.init_ok.MazePort);
			MazeWidth = ntohl(return_message.init_ok.MazeWidth);
			MazeHeight = ntohl(return_message.init_ok.MazeHeight);
			break;

		// If the server could not initialize with the given information
		case AM_INIT_FAILED:
			// Figure out which error message we got and print the appropriate message
			switch(ntohl(return_message.init_failed.ErrNum)){
				// Tell the user there are too many avatars
				case AM_INIT_TOO_MANY_AVATARS:
					fprintf(stderr, "AM_INIT_FAILED : too many avatars.\n");
					break;

				// Tell the user the difficulty was not in range
				case AM_INIT_BAD_DIFFICULTY:
					fprintf(stderr, "AM_INIT_FAILED : bad difficulty.\n");
					break;

				// Tell the user the error number of the unkown message
				default:
					fprintf(stderr, "AM_INIT_FAILED : ErrNum: 0x%08x\n",
						ntohl(return_message.init_failed.ErrNum));
			}

			// Closet the socket
			close(sock);

			// Return an exit status of 7
			return(7);

			// Break the outer switch statement (unreachable code, but good form)
			break;

		// If none of the above, tell the user we got a weird message and return
		default:
			fprintf(stderr,"Getting strange message back: 0x%08x\n",
				ntohl(return_message.type));
			return(7);
	}

	// Now that we have info, try to close AMStartup socket
 	if(close(sock) < 0){
		// if it didn't work, tell the user
 		perror("closing stream socket");

		// Return exit status 10
 		return(10);
 	}

	// Create a maze that will locally represent the server's maze
	maze = maze_create(MazeHeight, MazeWidth);

	// Initialize a thread for graphics
	pthread_t thread;

	// Create the thread for graphics
	pthread_create(&thread, NULL, gui_print_maze, maze);

	// Give XQuartz some time to start up
	printf("Waiting to allow graphics to load:\n");
	for(int i = 10; i>0; i--){
		printf("%d...", i);
		fflush(stdout);
		sleep(1);
	}
	printf("\n");

	// initialize the filename variable
	char filename[MAX_LEN];

	// Create the log file
	sprintf(filename,"Amazing_%s_%d_%d.log", getenv("USER"),
		ntohl(init_message.init.nAvatars),
		ntohl(init_message.init.Difficulty));

	// Tell the user where to find the log file
	printf("log written in %s\n", filename);

	// Get the current time
	time_t t = time(NULL);
	struct tm tim = *localtime(&t);

	// Open the newly created file for writing
	logfile = fopen(filename, "w");

	// Print the first line with relevant information to the log file
	fprintf(logfile, "%s, %d, %02d/%02d/%04d %02d:%02d:%02d\n", getenv("USER"),
		MazePort, tim.tm_mday, tim.tm_mon+1, tim.tm_year+1900, tim.tm_hour,
		tim.tm_min, tim.tm_sec);

	// For convenience, set some local variables to the message info
	int nAvatars = ntohl(init_message.init.nAvatars);
	int Difficulty = ntohl(init_message.init.Difficulty);

	// Allocate space for an array of threads, length nAvatars
	pthread_t *threads = malloc(nAvatars * sizeof(pthread_t));

	// Create a thread for each avatar
	for(int i = 0; i<nAvatars; i++){

		// Construct a thread_info structure with the data
		thread_info *info = create_info(i, nAvatars, Difficulty, hostname, MazePort,
			filename, MazeWidth, MazeHeight);

		// Make sure it worked
		if(info == NULL){
			// Or tell the user the malloc failed and return exit status 8
			fprintf(stderr, "malloc failed.\n");
			return(8);
		}

		// Try to create all of the threads
		int stat = pthread_create(&threads[i], NULL, client, info);

		// If we couldn't create the threads, tell the user and return exit status 9
		if(stat){
			fprintf(stderr, "pthread_create failed, rc=%d\n", stat);
			return(9);
		}
	}

	// Make main wait for the threads to finish by joining all of the threads
	for(int i = 0; i<nAvatars; i++) {
		pthread_join(threads[i], NULL);
	}

	// Join the graphics thread
	pthread_join(thread, NULL);

	// Now that the maze has been solved, clean up
 	fclose(logfile);
 	free(threads);
 	maze_delete(maze);

	// Return success
 	return(0);
}


/* ******************************************************************
* Name: create_info
*
* Description: creates a malloc'd struct with all of the maze info in it to
* 	pass to each individual avatar thread
*
* Input: int AvatarID, the ID of the Avatar
*				 uint32_t nAvatars, the number of avatars
*				 uint32_t Difficulty, the difficulty of the maze
*				 char* hostname, the server hostname
*		 		 uint32_t MazePort, the port on which the maze is run
*				 char* logname, the filename for the logfile
*				 uint32_t MazeWidth, the width of the maze
*				 unint32_t MazeHeight, the height of the maze
*
* Return: thread_info, the struct with all of the data in it
*
*
*/
thread_info *create_info(int AvatarId, uint32_t nAvatars, uint32_t Difficulty,
		char *hostname, uint32_t MazePort, char *logname, uint32_t MazeWidth,
		uint32_t MazeHeight)
{
	// Try to malloc the thread info
	thread_info *new = malloc(sizeof(thread_info));

	// Check if the malloc succeeded
	if(new == NULL){
		// If it failed, print out the error to the user
		fprintf(stderr,"Malloc failed for thread_info %d.\n", AvatarId);

		// Return nothing to the caller to indicate an error
		return NULL;
	}

	// Set each of the struct's children to the passed in values
	new->AvatarId = AvatarId;
	new->nAvatars = nAvatars;
	new->Difficulty = Difficulty;
	new->MazePort = MazePort;
	new->MazeHeight = MazeHeight;
	new->MazeWidth = MazeWidth;

	// Try to malloc the hostname
	new->hostname = malloc(sizeof(char) * (strlen(hostname) +1));

	// Check if the malloc succeeded
	if(new->hostname == NULL){
		// If it failed, print out the error to the user
		fprintf(stderr,"Malloc failed for hostname in avatar %d.\n", AvatarId);

		// free the malloc'd struct
		free(new);

		// Return nothing to the caller to indicate an error
		return NULL;
	}

	// Copy the hostname into the malloc'd position
	strcpy(new->hostname, hostname);

	// Try to malloc the log name
	new->logname = malloc(sizeof(char) * (strlen(logname) + 1));

	// Check if the malloc succeeded
	if(new->logname == NULL){
		// If it failed, print out the error to the user
		fprintf(stderr,"Malloc failed for logname in avatar %d.\n", AvatarId);

		// free the malloc'd struct and the malloc'd hostname
		free(new->hostname);
		free(new);

		// Return nothing to the caller to indicate an error
		return NULL;
	}

	// Copy the logname into malloc'd postion
	strcpy(new->logname, logname);

	// All went well so return the created struct to the caller
	return(new);
}


/* ******************************************************************
* Name: delete_info
*
* Description: frees the memory malloc'd by create_info
*
* Input: thread_info *info, a pointer to the struct to free
*
* Return: (void)
*
*
*/
void delete_info(thread_info *info)
{
	// Make sure the struct exists
	if(info != NULL){
		// Free the hostname
		free(info->hostname);

		// Free the logname
		free(info->logname);

		// Free the overarching struct
		free(info);
	}
}
