/* ========================================================================== */
/* File: amazing.h
 *
 * Project name: CS50 Amazing Project
 * Component name: amazing header
 *
 * Primary Author: Charles C. Palmer
 * Date Created: Mon Jan 2 2012
 *
 * Last Update by: Mazed and Confused - Sunday, 28 August 2016
 *
 * Updated by Xia Zhou, August 2016
 *
 * Updated by Mazed and Confused, August 2016
 *
 * This file contains the common defines for the Amazing Project.
 *
 */
/* ========================================================================== */
#ifndef __AMAZING_H
#define __AMAZING_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdint.h>                          // uint32_t
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>         //socket-related structures
#include <string.h>         //memcpy, memset
#include <errno.h>          //strerror
#include <unistd.h>         //getopt
#include <getopt.h>         //getopt_long
#include <stdbool.h>        //bool type
#include <pthread.h>


// ---------------- Constants
/* Server parameters */
#define AM_SERVER_PORT  "17235"              // Server management port, ASCII "CS"
#define AM_MAX_MESSAGE     128               // max message length in bytes
#define AM_MAX_DIFFICULTY    9               // max # of maze difficulty levels
#define AM_MAX_AVATAR       10               // max # of avatars for any MazePort
#define AM_MAX_MOVES      1000               // max # of moves for all Avatars
#define AM_WAIT_TIME       600               // seconds waited before server dies

/* Avatar constants */
#define M_WEST           0
#define M_NORTH          1
#define M_SOUTH          2
#define M_EAST           3
#define M_NUM_DIRECTIONS 4
#define M_NULL_MOVE      8

/* Client-server protocol message type values */
#define AM_ERROR_MASK             0x80000000
#define AM_MAZE_SOLVED            0x00010000
#define AM_INIT                   0x00000001
#define AM_INIT_OK                0x00000002
#define AM_AVATAR_READY           0x00000004
#define AM_AVATAR_TURN            0x00000008
#define AM_AVATAR_MOVE            0x00000010
#define AM_INIT_FAILED           (0x00000020 | AM_ERROR_MASK)
#define AM_AVATAR_OUT_OF_TURN    (0x00000040 | AM_ERROR_MASK)
#define AM_NO_SUCH_AVATAR        (0x00000080 | AM_ERROR_MASK)
#define AM_TOO_MANY_MOVES        (0x00000100 | AM_ERROR_MASK)
#define AM_SERVER_DISK_QUOTA     (0x00000200 | AM_ERROR_MASK)
#define AM_SERVER_OUT_OF_MEM     (0x00000400 | AM_ERROR_MASK)
#define AM_UNKNOWN_MSG_TYPE      (0x01000000 | AM_ERROR_MASK)
#define AM_SERVER_TIMEOUT        (0x02000000 | AM_ERROR_MASK)
#define AM_UNEXPECTED_MSG_TYPE   (0x04000000 | AM_ERROR_MASK)

/* AM_INIT failure ErrNums */
#define AM_INIT_ERROR_MASK       (0x40000000 | AM_ERROR_MASK)
#define AM_INIT_TOO_MANY_AVATARS (0x00000001 | AM_INIT_ERROR_MASK)
#define AM_INIT_BAD_DIFFICULTY   (0x00000002 | AM_INIT_ERROR_MASK)

// ---------------- Structures/Types
/* XY-coordinate position */
typedef struct XYPos
{
    uint32_t x;
    uint32_t y;
} XYPos;

/* Maze avatar */
typedef struct Avatar
{
    int fd;
    XYPos pos;
} Avatar;

/* AM Message description */
typedef struct AM_Message
{
    uint32_t type;

    /* Define a union for all the message types that have parameters. Messages
     * with no parameters don't need to be part of this union. Defined as an
     * anonymous union to facilitate easier access.
     */
    union
    {
        /* AM_INIT */
        struct
        {
            uint32_t nAvatars;
            uint32_t Difficulty;
        } init;

        /* AM_INIT_OK */
        struct
        {
            uint32_t MazePort;
            uint32_t MazeWidth;
            uint32_t MazeHeight;
        } init_ok;

        /* AM_INIT_FAILED */
        struct
        {
            uint32_t ErrNum;
        } init_failed;

        /* AM_AVATAR_READY */
        struct
        {
            uint32_t AvatarId;
        } avatar_ready;

        /* AM_AVATAR_TURN */
        struct
        {
            uint32_t TurnId;
            XYPos    Pos[AM_MAX_AVATAR];
        } avatar_turn;

        /* AM_AVATAR_MOVE */
        struct
        {
            uint32_t AvatarId;
            uint32_t Direction;
        } avatar_move;

        /* AM_MAZE_SOLVED */
        struct
        {
            uint32_t nAvatars;
            uint32_t Difficulty;
            uint32_t nMoves;
            uint32_t Hash;
        } maze_solved;

        /* AM_UNKNOWN_MSG_TYPE */
        struct
        {
            uint32_t BadType;
        } unknown_msg_type;
    };
} AM_Message;

// ---------------- Public Variables
/* add your variables here if any*/

/* thread_info : relavent information to pass to the thread
 *
 * As of changes committed 8/28/2016, this structure is likely redundant.
 *	kept as part of the code museum!
 */
typedef struct info{
	int AvatarId;
	uint32_t nAvatars;
	uint32_t Difficulty;
	char *hostname;
	uint32_t MazePort;
	char *logname;
	uint32_t MazeHeight;
	uint32_t MazeWidth;
} thread_info;

/* cell_t : individual cell in a maze.
 *	may have 4 borders and an avatar within it
 */
typedef struct{
	int north;	//0 means no wall, 1 means wall
	int south;
	int east;
	int west;
	int has_avatar;
} cell_t;

/* maze_t : maze data structure
 *	contains a 2D array of cells, as well as
 *	variables holding width and height for convenience
 */
typedef struct{
	cell_t **plot;
	int MazeWidth;
	int MazeHeight;
} maze_t;

// ---------------- Prototypes/Macros
#define IS_AM_ERROR(code) ((code) & (AM_ERROR_MASK))
#define MAX_LEN 1000		//string length may be at most 1000 characters

extern FILE *logfile;


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
		uint32_t MazeHeight);


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
void delete_info(thread_info *info);

/* *****************************************************************************
* Name: maze_create
*
* Description: Creates the maze with the local datastructures, mallocing as
* 	appropriate and returning the local datastructure to the caller. Maze_create
*		allocates heap memory for a new maze_t data structure. The user is
*		responsible for freeing this memory later on.
*
* Input: int MazeHeight, height of the maze to create
*		 		 int MazeWidth, MazeWidth
*
* Return: maze_t, the maze created by the function
*
*/
maze_t *maze_create(int MazeHeight, int MazeWidth);


/* *****************************************************************************
* Name: maze_delete
*
* Description: delete a maze_t data structure and clean up its contents
*
* Input: maze_t *maze, a pointer to the struct to free
*
* Return: (void)
*
*
*/
void maze_delete(maze_t *maze);


/* *****************************************************************************
* Name: add_wall
*
* Description: add a wall to the maze_t data structure
*
* Input: int AvatarID, the ID of the Avatar
*				 maze_t *maze, pointer to the maze struct
*				 char nsew, character representing where to place the wall
*				 int i, the Y coordinate to place the wall
*		 		 int j, the X coordinate to place the wall
*
* Return: (void)
*
*
*/
void add_wall(maze_t *maze, char nsew, int i, int j);


/* *****************************************************************************
* Name: set_avatar
*
* Description: specify that a cell contains an avatar
*
* Input: maze_t *maze, pointer to the maze struct
*				 int i, the Y coordinate to place the wall
*		 		 int j, the X coordinate to place the wall
*
* Return: (void)
*
*
*/
void set_avatar(maze_t *maze, int i, int j);


/* *****************************************************************************
* Name: remove_avatar
*
* Description: remove an avatar from a cell
*
* Input: maze_t *maze, pointer to the maze struct
*				 int i, the Y coordinate to place the wall
*		 		 int j, the X coordinate to place the wall
*
* Return: (void)
*
*
*/
void remove_avatar(maze_t *maze, int i, int j);


/* *****************************************************************************
* Name: print_maze
*
* Description: print the contents of a maze_t data structure to the terminal
*
* Input: maze_t *maze, pointer to the maze struct
*
* Return: (void)
*
*
*/
void print_maze(maze_t *maze);


/* *****************************************************************************
* Name: gui_print_maze
*
* Description: print the contents of the maze using a graphical user interface.
*	gui_print_maze will continue to update the maze display as the solver updates
*	the data structure.
*
*	NOTE: In order for gui_print_maze to function properly it must run on separate
*	thread from the rest of the master program. Below is an example of the correct
*	usage:
*
*		pthread_t thread;
* 		pthread_create(&thread, NULL, gui_print_maze, maze);
*
* Input: void *maze, a pointer to the maze to print
*
* Return: void *, NULL
*
*
*/
void *gui_print_maze(void *maze);


/* *****************************************************************************
* Name: logMove
*
* Description: function to write each move or attempted move to the log file
*		AM_Message message is message received from server
*
* Input: FILE *file is the logfile name
*				 int current_avatar, the ID of the avatar whose turn we want to log
*				 int move_sent, the move sent to the server
*
* Return: (void)
*
*
*/
void logMove(FILE *file, int current_avatar, int move_sent, XYPos position);


/* *****************************************************************************
* Name: message_error
*
* Description: function to determine if a message from the server is one of many
*		types of error messages AM_Message message - message received from server to
*		be checked
*
* Input: FILE *file is the logfile name
*				 int socket, socket to close if there is an error
*
* Return: (void)
*
*
*/
int message_error(AM_Message message, FILE *file, int socket);


/* *****************************************************************************
* Name: make_move
*
* Description: function to send move to server from the client
*
* Input: int current_avatar is the avatar ID of the current avatar
*				 int move,the direction to move
*				 int socket, int of the socket you want to close
*
* Return: (void)
*
*
*/
void make_move(int current_avatar, int moveToSend, int socket, FILE *fp);


/* *****************************************************************************
* Name: getMove
*
* Description: Performs the Left-Hand Algorithm to determine each avatar's next
*		move.
*
* Input: XYPos avatarZeroPos, position of avatar 0
*				 XYPos prevPos, previous position of current avatar
*				 XYPos currPos, current position of current avatar
*				 int prevMove, the previous move the current avatar maze_delete
*				 int avatarID, the curretn avatar's ID, included for testing
*
* Return: int, representing the move to make for the current avatar
*
*
*/
int getMove(XYPos avatarZeroPos, XYPos prevPos, XYPos currPos, int prevMove, int avatarID);

#endif // __AMAZING_H
