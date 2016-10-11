DESIGN.md
Design Documentation
Version 2 (FINAL) 
Edited from original version submitted on August 20, 2016

Team Mazed and Confused
Andrew Werchniak, Robert Sayegh, and Cristina Curcelli

CS50 Summer 2016

AMStartup, amazing_client.c, and amazing.c

===============================================================================

Consulted Lecture 15 and The Maze Challenge assignment on the CS 50 website 
in the making of this document.

1.	Input: Any inputs to the module
2.	Output: Any outputs of the module
3.	Data Flow: Any data flow through the module
4.	Data Structures: Major data structures used by the module
5.	Pseudo Code: Pseudo code description of the module.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AMStartup.c

1. Input

The only inputs are command-line parameters, as follows:

-n nAvatars: (int) the number of Avatars in the maze
-d Difficulty: (int) the difficulty level, on the scale 0 (easy) 
	to 9 (excruciatingly difficult)
-h Hostname: (char *) the hostname of the server. The server will be running 
	on flume.cs.dartmouth.edu.

User interface:

	./AMStartup [-n nAvatars] [-d Difficulty] [-h hostname]

Example:    	

	./AMStartup –n 2 –d 0 –h flume.cs.dartmouth.edu 

===============================================================================

2. Output: the program will create N avatars as threads with a call 
to the client function, producing a log file with logged moves up 
until the maze is solved, ASCII graphics printed in the output, 
and a GUI of the avatars running through the maze.
	
===============================================================================

3. Data Flow:

We made use of the following modules or functions:

1.	main, which validates command line parameters, creates a socket, 
establishes and confirms a connection to the server, creates a log file, 
and creates threads

2.	client, which is written in the amazing_client.c program, and detailed 
in depth in the amazing_client.c documentation

3.	maze_create, function to create the data structure to represent the maze 
locally as a 2D array. 

4.	create_info, function to create a thread_info struct, which we define 
in the header file amazing.h, which allows us to store the info 
that the client function will need. 

Dataflow through modules:

1.	main validates command line parameters and creates the initialization 
message accordingly with appropriate number of avatars and difficulty; 
main then creates a socket, initializes the fields of the server address, 
and connects the socket to the server; main then confirms the connection 
with the server by sending and receiving the initialization message. 
Main also sets up GTK graphics. 

2.	maze_create is called to create our maze data structure that will 
represent the maze locally (maze_create is written in amazing.c)

3.	create_info is called, within main, to create a variable holding 
all information needed when creating threads for each avatar

4.	client is called when creating threads for each avatar, initiating a call 
to the client function for each avatar, where the avatars continue moving 
until the maze is solved. 

===============================================================================

4. Data Structures: 

A thread_info struct, which stores information relevant to the client.


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

===============================================================================

5. Pseudo Code for logic/algorithmic flow:

The AMStartup will follow the following outline:

1.	execute from a command line with usage syntax
	
	• ./AMStartup [-n nAvatars] [-d difficulty] [-h hostname]
	• where –n nAvatars (int) is the number of Avatars in the maze
	• where –d difficulty (int) is the difficulty level, on the scale 0 (easy) 
		to 9 (excruciatingly difficult)
	• where –h hostname (char *) the hostname of the server. The server will 
		be running on flume.cs.dartmouth.edu.

2.	validate command line parameters, using getopt switches 
	a. while validating parameters, initiate the init message to later send 
	to the server

3.	create a socket

4.	initialize the fields of the server address, using the hostname command 
	line parameter

5.	connect the socket to that server

6.	send the initialization message to the server

7.	receive a message back from the server

8.	after sending and receiving…
	
	a. if the return message says the initialization was successful, 
	extract MazePort, MazeWidth, and MazeHeight from the return message
	
	b. if the return message says initialization was unsuccessful,  check which
	parameter was out-of-bounds

9.	create a maze locally, using MazeHeight and MazeWidth, with a call to a 
function that will set up our maze data structure (defined in amazing_client.c)

10.	create a thread where the GTK gui will run

11.	create and open a log file named as follows:
			Amazing_$USER_N_D.log
    with first line containing $USER, MazePort, and date/time,
   	to log moves made by the avatars as they move through the maze

12.	allocate memory and create threads for each avatar 
	a. when creating threads pass the client function as a parameter, 
	which will start a call to client for each avatar, allowing it to start 
	making moves through the maze. 

13.	close the socket connection when done

14.	cleanup threads

15.	close logfile

16.	delete the local maze

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

amazing_client.c

1. Input: The function void client(void *arg), the one and only function in 
amazing_client.c, called by each avatar’s thread, takes no input parameters. 

===============================================================================

2. Output: client adds walls to the ASCII graphics when the avatars discover 
walls in the maze, and client logs moves in the log file for each avatar’s 
moves.    

===============================================================================

3. Data Flow:

We will make use of the following modules or functions: 

1.	client
Usage: takes no parameters, is called in pthread_create by each avatar, 
allows avatars to make moves until maze is solved. 

2.	Message_error
Usage: void message_error(AM_Message message, FILE *file, int socket)
	where message is a return message received from the server, file is the 
	logfile, and socket is the socket opened in client. 

In the while loop, after receiving a message, message_error is called to check 
if the received message contains one of many possible errors from the server as 
defined in cs50 project specifications. 

3.	getMove
Usage: int getMove(XYPos avatarZeroPos, XYPos prevPos, XYPos currPos, 
	\int prevMove, int avatarID)
	where avatarZeroPos is the position of the avatar that stays stationary, 
	prevpos is the current avatar’s previous position, currPos is the current 
	avatar’s current position, prevMove is the current avatar’s previous 
	position, previous move is the last move the avatar made, and the avatarID 
	is the current avatar’s ID. 

Once we confirm that the return message is an AM_AVATAR_TURN message, we make a 
call to getMove to calculate the int representation of what move the avatar 
needs to make now. 
 
4.	add_wall
Usage: void add_wall(maze_t *maze, char nsew, int i, int j)
	where maze is the local maze data structure, nsew is the character 
	representing the direction where the wall must be added relative to 
	the (i,j) position, and i and j are the int representations of a cell 
	position, such that maze[i][j] in the maze data structure represents 
	the cell of the position.

We compare the current position of the avatar against its previous position 
to tell if it has moved. In the case where it hasn’t moved, we know that the 
server returned that the move was not possible and therefore there is a wall 
there. We check the direction of the previous move, and then call add_wall. 
 
5.	make_move
Usage: void make_move(int current_avatar, int moveToSend, int socket, FILE *fp)
	where current_avatar is the current avatar ID, moveToSend is the direction 
	the avatar will attempt a move, socket is the socket set up by the client, 
	and fp is the logfile to log the move. 

The avatar sends a move to the server using make_move.
 
6.	log_move
Usage: void logMove(FILE *file, int current_avatar, int move_sent, 
		\XYPos position)
	where file is the logfile, current_avatar is the avatar ID, move_sent is 
	the direction the avatar attempted to move, and position is the avatar’s 
	current posisiton. 

After sending a move to the server using make_move, log_move is called to log 
the attempted move. 

7.	remove_avatar
Usage: void remove_avatar(maze_t *maze, int i, int j)
	where the maze is the local maze data structure, and i and j are the int 
	representations of a cell position in the maze data structure. 

After the move is made, remove_avatar is called to remove the avatar in the 
local maze representation from it’s previous position in the maze. 

8.	set_avatar
Usage: void set_avatar(maze_t *maze, int i, int j)
	where the maze is the local maze data structure, and i and j are the int 
	representations of a cell position in the maze data structure. 

After the avatar is removed from the previous position in the maze, set_avatar 
is called to add an avatar at the current position in the local maze 
representation. 

9.	print_maze
Usage: void print_maze(maze_t *maze)
	where maze is the local maze data structure. 

After the avatar position has been updated in the local maze, print_maze is 
called to print a new ASCII representation of the new maze.

===============================================================================

4. Data Structures:

cell_t : individual cell in a maze. May have 4 borders and an avatar within it

typedef struct{
	int north;	//0 means no wall, 1 means wall
	int south;
	int east;
	int west;
	int has_avatar;
} cell_t;

maze_t : maze data structure, contains a 2D array of cells, as well as, 
variables holding width and height for convenience

typedef struct{
	cell_t **plot;
	int MazeWidth;
	int MazeHeight;
} maze_t;

===============================================================================

5. Pseudo Code for logic/algorithmic flow:  

The amazing_client.c will follow the following outline:
1.	Create a socket on the client side

2.	Initialize server fields 

3.	Connect the socket to that server

4.	Send an AM_AVATAR_READY message to the server 

5.	While the maze has not been solved
	a. Receive a message from the server 
	b. Check the message to see if it is an error 
	c. Check if the message is an AM_AVATAR_TURN message
		1. Keep track of current position
		2. Keep track of the position of the 0th avatar, the one who will stay 
		stationary
		3. Calculate the move to make based on the left-hand-rule algorithm
		4. Check if we hit a wall on the last move
			1. If so add a wall
		5. Make a move
		6. Log a move
		7. update the local maze
		8. adjust previous position to be current 
			d. check if the message is an AM_MAZE_SOLVED message

6. close the socket

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

