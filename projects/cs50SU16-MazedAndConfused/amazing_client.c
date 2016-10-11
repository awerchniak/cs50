/* ************************************************************************** */
/* File: amazing_client.c
 *
 * Project name: CS50 Amazing Project
 * Component name: amazing client file
 *
 * Primary Author: Mazed and Confused
 * Date Created: August 2016
 *
 * Last Update by: Mazed and Confused - Sunday, 28 August 2016
 *
 * This file contains the function definition for the amazing client in the Amazing Project.
 *
 */

/*********************************** IMPORTS **********************************/
#include "amazing.h"										//AM_INIT, etc.
#include <stdlib.h>											//getenv(), malloc()
#include <unistd.h>											//getopt
#include <stdio.h>											//printf, etc.
#include <stdbool.h>										//bool type
#include <netdb.h>											// socket-related structures
#include <string.h>											//memcpy, memset
#include <stdint.h>											//unint32_t, etc.
#include <errno.h>											//strerror
#include <arpa/inet.h>

/* *****************************************************************************
* Name: client
*
* Description:  Function to drive the behavior of each individual avatar. To be run
*				concurrently with multiple avatars to solve the maze
*
* Input: arg : a pointer to a struct of information to pass to the avatar
*
* Return: NULL void pointer
*
*/
void *client(void *arg){

	//grab global variables from AMStartup
	extern FILE *logfile;
	extern maze_t *maze;
	extern pthread_mutex_t mutex1;

	//assign parameter into local variables for convenience
	int avatarID = ((thread_info *)arg) -> AvatarId;
	char * hostname = ((thread_info *)arg)->hostname;
	uint32_t mazeport = ((thread_info *)arg)->MazePort;

	//declare and initialize variables for later
	int unsolved = 1;
	int prevMove = M_NULL_MOVE;
	XYPos currPos;;
	currPos.x = -1;
	currPos.y = -1;
	XYPos prevPos;
	prevPos.x = -1;
	prevPos.y = -1;
	XYPos avatarZeroPos;
	prevPos.x = -1;
	prevPos.y = -1;


	//creating a socket on the client side
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	if (sock < 0){
  		perror("Error opening socket");
  		return NULL;
	}

	//get the host by its name
	struct hostent *host;
	if (( host = gethostbyname(hostname)) == NULL){
    	perror("Invalid hostname");
    	return NULL;
  	}

	//get the host address
  	struct in_addr **myInAddr = (struct in_addr **) host->h_addr_list;

	//get the IP address
  	char *IPAddress= inet_ntoa(*myInAddr[0]);

	//declare the server struct
	struct sockaddr_in server;

	//fill the server struct
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(mazeport);
	server.sin_addr.s_addr = inet_addr(IPAddress);

 	//connect the socket to that server
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		perror("Client: connecting stream socket");
		return NULL;
	}


	//send avatar ready message
	AM_Message ready_message = {.type = htonl(AM_AVATAR_READY), .avatar_ready.AvatarId = htonl(avatarID)};

	if (write(sock, &ready_message, sizeof(AM_Message)) < 0){
		perror("sending avatar ready message");
      	return NULL;
	}

	//print message to the logfile
	fprintf(logfile,"Avatar #%d sent AM_AVATAR_READY signal\n", ntohl(ready_message.avatar_ready.AvatarId));

	//continue until the maze is solved
	while (unsolved){

		AM_Message return_message;

		int move_to_make; 				//move to make, based on heuristics

		//read the return message, which should be an AM_AVATAR_TURN message
		if(read(sock, &return_message, sizeof(AM_Message)) < 0){
			delete_info(arg);
			perror("client reading return message from socket");
			exit(1);
		}

		//void message_error(AM_Message message, FILE *file, int socket)
		if (message_error(return_message, logfile, sock) > 0) {
			delete_info(arg);
			return NULL;
		}

		//if it's an avatar's turn
		if (return_message.type == htonl(AM_AVATAR_TURN)) {

			//if it's this avatar's turn
			if (ntohl(return_message.avatar_turn.TurnId) == avatarID) {

				//get the position of this avatar
				currPos = return_message.avatar_turn.Pos[avatarID];
				
				//get the position of avatar 0
				avatarZeroPos = return_message.avatar_turn.Pos[0];

				//call getMove to figure out where to move
				move_to_make = getMove(avatarZeroPos, prevPos, currPos, prevMove, avatarID);

				// update local maze struct; if we didnt move add a wall
				if ((prevPos.x == currPos.x && prevPos.y == currPos.y) && (avatarZeroPos.x != currPos.x && avatarZeroPos.y != currPos.y)) {
					if (prevMove == M_NORTH) {
						add_wall(maze, 'n', ntohl(currPos.y), ntohl(currPos.x));
					} else if (prevMove == M_SOUTH) {
						add_wall(maze, 's', ntohl(currPos.y), ntohl(currPos.x));
					} else if (prevMove == M_WEST) {
						add_wall(maze, 'w', ntohl(currPos.y), ntohl(currPos.x));
					} else if (prevMove == M_EAST) {
						add_wall(maze, 'e', ntohl(currPos.y), ntohl(currPos.x));
					}
				}

				//make that move
				make_move(avatarID, move_to_make, sock, logfile);
				//log the move
				logMove(logfile, avatarID, move_to_make, currPos);

				//update the maze data structure
				remove_avatar(maze, ntohl(prevPos.y), ntohl(prevPos.x));
				set_avatar(maze, ntohl(currPos.y), ntohl(currPos.x));

				//update previous move log
				prevMove = move_to_make;
				prevPos.x = currPos.x;
				prevPos.y = currPos.y;

				//important: lock permission while printing the maze so multiple avatars
				//	don't attempt to print at once and cause errors
				pthread_mutex_lock(&mutex1);
				print_maze(maze);
				pthread_mutex_unlock(&mutex1);

			}
		//if we solved the maze
		} else if (return_message.type == htonl(AM_MAZE_SOLVED)) {
			//print to logfile
			fprintf(logfile, "\n%s\n", "AM_MAZE_SOLVED message received!");

			fprintf(logfile, "%s: %d %s: (%d, %d)\t ", "AvatarID", avatarID, "End position", ntohl(currPos.x), ntohl(currPos.y));
			fprintf(logfile, "\n%s: %d\n %s: %d\n %s: %d\n %s: %d\n", "Number of avatars", ntohl(return_message.maze_solved.nAvatars),\
				"Level of difficulty", ntohl(return_message.maze_solved.Difficulty),\
				"Number of moves", ntohl(return_message.maze_solved.nMoves),\
				"Hash", ntohl(return_message.maze_solved.Hash));
			//the maze is now solved!
			unsolved = 0;
	
			//free memory from input arguments
			delete_info(arg);
		}
	}

	//close the socket when we're done
	if(close(sock) < 0){
		perror("closing thread socket");
	}

	//return null pointer
	return NULL;
}
