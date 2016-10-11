/* ************************************************************************** */
/* File: amazing.c
 *
 * Project name: CS50 Amazing Project
 * Component name: amazing file
 *
 * Primary Author: Mazed and Confused
 * Date Created: August 2016
 *
 * Last Update by: Mazed and Confused - Sunday, 28 August 2016
 *
 * This file contains the function definitions for amazing.h in the Amazing Project.
 *
 * Certain GUI helper functions borrow examples from the GTK tutorial found here:
 *	https://developer.gnome.org/gtk3/stable/ch01s05.html.
 * Relevant citations given in function headers.
 *
 */

/*********************************** IMPORTS **********************************/
#include <stdio.h>														//printf
#include <stdlib.h>														//malloc
#include <netdb.h>														//phread
#include <gtk/gtk.h>													//gtk

/******************************** LOCAL IMPORTS *******************************/
#include "amazing.h"

/************************* GLOBAL VARIABLE DEFINITIONS ************************/
static cairo_surface_t *surface = NULL;				//surface to display maze
GtkWidget *window;														//window to display GUI
GtkWidget *frame;															//frame for Gtk Drawing Area
GtkWidget *drawing_area;											//area to display maze on


/************************** LOCAL FUNCTION PROTOTYPES *************************/
static void clear_surface (void);
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event,
	gpointer data);
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data);
static void draw_maze (GtkWidget *widget, gpointer data);
static int timer(void* data);
static void close_window (void);
static void activate (GtkApplication *app, gpointer user_data);


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
maze_t *maze_create(int MazeHeight, int MazeWidth)
{
	// Try to malloc space for the maze
	maze_t *new = malloc(sizeof(maze_t));

	// Check if the malloc worked
	if(new == NULL){
		// If it failed, print an error and return NULL to signal it to the caller
		fprintf(stderr, "error with malloc\n");
		return NULL;
	}

	// Try to malloc data for the plot itself
	new->plot = malloc(sizeof(cell_t *) * MazeHeight);

	// Check to make sure that malloc worked
	if(new->plot == NULL){
		// Print the error if it failed
		fprintf(stderr, "error with malloc\n");

		// Free the previous memory
		free(new);

		// Return NULL to the caller to signal the error
		return(NULL);
	}

	// Try to allocate memory for each row in the maze
	for(int i = 0; i< MazeHeight; i++){
		// Try to make the allocation
		new->plot[i] = malloc(sizeof(cell_t) * MazeWidth);

		//Check to see if it worked
		if(new->plot[i] == NULL){
			// If failed, print the error
			fprintf(stderr, "error with malloc\n");

			// Free previously malloc'd memory
			for(int j = 0; j<i; j++){
				free(new->plot[j]);
			}
			free(new->plot);
			free(new);

			// Return NULL to the caller to signal the error
			return NULL;
		}

		// If it worked
		else {
			// Initialize all the values to 0
			for(int j = 0; j< MazeWidth; j++){
				new->plot[i][j].north = 0;
				new->plot[i][j].south = 0;
				new->plot[i][j].east = 0;
				new->plot[i][j].west = 0;
				new->plot[i][j].has_avatar = 0;
			}
		}
	}

	// Set the width and height of the new maze struct
	new->MazeWidth = MazeWidth;
	new->MazeHeight = MazeHeight;

	// Set the border walls
	for(int i = 0; i <MazeHeight;i++){
		for(int j = 0; j < MazeWidth; j++){

			// Set top borders
			if(i == 0)
				add_wall(new,'n',i,j);

			// Set bottom borders
			if(i == MazeHeight - 1)
				add_wall(new,'s',i,j);

			// Set left borders
			if(j == 0)
				add_wall(new,'w',i,j);

			// Set right borders
			if(j == MazeWidth - 1)
				add_wall(new,'e',i,j);
		}
	}

	// All went well, so return the new maze to the caller
	return(new);
}


/* ******************************************************************
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
void maze_delete(maze_t *maze)
{
	// First validate parameters : do not free null pointers!
	if (maze!= NULL){
		//Free each row in the plot
		for(int i = 0; i<maze->MazeHeight; i++){
			free(maze->plot[i]);
		}

		// Free the entire plot
		free(maze->plot);

		// Free the maze
		free(maze);
	}
}


/* ****************************************************************************************
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
void add_wall(maze_t *maze, char nsew, int i, int j)
{
	// Check input parameters. exit on failure
	if(maze == NULL || maze->plot == NULL){
		fprintf(stderr, "null maze!\n");
		return;
	}

	if(i >= maze->MazeHeight || i<0){
		fprintf(stderr, "Error! Row index out of bounds.\n");
		return;
	}

	if(j >= maze->MazeWidth || j<0){
		fprintf(stderr, "Error! Col index out of bounds.\n");
		return;
	}

	// Add appropriate wall
	switch(nsew){
		// North wall
		case 'n':
			// Add the north wall
			maze->plot[i][j].north = 1;

			// Set one above it's southern border
			if(i>0)
				maze->plot[i-1][j].south = 1;

			// Break from the switch statement
			break;

		// North wall
		case 'N':
			// Add the north wall
			maze->plot[i][j].north = 1;

			// Set one above it's southern border
			if(i>0)
				maze->plot[i-1][j].south = 1;

			// Break from the switch statement
			break;

		// East wall
		case 'e':
			// Add the east wall
			maze->plot[i][j].east = 1;

			// Set adjacent's western border
			if(j<maze->MazeWidth-1)
				maze->plot[i][j+1].west = 1;

			// Break from the switch statement
			break;

		// East wall
		case 'E':
			// Add the east wall
			maze->plot[i][j].east = 1;

			// Set adjacent's western border
			if(j<maze->MazeWidth-1)
				maze->plot[i][j+1].west = 1;

			// Break from the switch statement
			break;

		// South wall
		case 's':
			// Set the south wall
			maze->plot[i][j].south = 1;

			//Set one below it's northern border
			if(i<maze->MazeHeight-1)
				maze->plot[i+1][j].north = 1;

			// Break from the switch statement
			break;

		// South wall
		case 'S':
			// Set the south wall
			maze->plot[i][j].south = 1;

			//Set one below it's northern border
			if(i<maze->MazeHeight-1)
				maze->plot[i+1][j].north = 1;

			// Break from the switch statement
			break;

		// West wall
		case 'w':
			// Add the west wall
			maze->plot[i][j].west = 1;

			// Set adjacent's eastern border
			if(j>0)
				maze->plot[i][j-1].east = 1;

			// Break from the switch statement
			break;

		// Western wall
		case 'W':
			// Add the west wall
			maze->plot[i][j].west = 1;

			// Set adjacent's eastern border
			if(j>0)
				maze->plot[i][j-1].east = 1;

			// Break from the switch statement
			break;

		// Error case
		default:
			// Print the error for the user to see
			fprintf(stderr,"error! nsew not a valid char.\n");

			// return nothing to end execution
			return;
	}
}


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
void set_avatar(maze_t *maze, int i, int j)
{
	// Validate the the maze exists
	if(maze == NULL){
		fprintf(stderr, "Error! Empty maze.\n");
		return;
	}

	// Ensure the row is in the maze, upper bound
	if(i >= maze->MazeHeight){
		fprintf(stderr, "Error! Row index out of bounds.\n");
		return;
	}

	// Ensure the Column is in the maze, upper bound
	if(j >= maze->MazeWidth){
		fprintf(stderr, "Error! Col index out of bounds.\n");
		return;
	}

	// Ensure columns and rows are in the maze lower bound
	if (i < 0 || j < 0) {
		return;
	}

	// Put an avatar on the cell
	maze->plot[i][j].has_avatar = 1;
}


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
void remove_avatar(maze_t *maze, int i, int j)
{
	// Make sure the maze exists
	if(maze == NULL){
		fprintf(stderr, "Error! Empty maze.\n");
		return;
	}

	// Maze sure the row is in the maze upper bound
	if(i >= maze->MazeHeight){
		fprintf(stderr, "Error! Row index out of bounds.\n");
		return;
	}

	// Make sure the column is in the maze lower bound
	if(j >= maze->MazeWidth){
		fprintf(stderr, "Error! Col index out of bounds.\n");
		return;
	}

	// Make sure the rows and columns are in the maze lower bound
	if (i < 0 || j < 0) {
		return;
	}

	// Remove the avatar from the maze
	maze->plot[i][j].has_avatar = 0;
}


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
void print_maze(maze_t *maze)
{
	// Make sure the maze exists
	if (maze == NULL){
		fprintf(stderr,"Empty maze.\n");
		return;
	}

	// Print blank lines so that terminal printout is less confusing
	printf("\n\n\n");

	// Print out the ASCII maze
	for(int i = 0; i<2*maze->MazeHeight+1; i++){			//rows
		for(int j =0; j<2*maze->MazeWidth+1; j++){			//cols
			if(i%2 ==0){																	//i%2==0 means a +-+-+ row
				if(j%2 ==0 && j!= 2*maze->MazeWidth-1)
					printf("+");
				else
					//test to see if we need a -. If not, do a space
					if((i/2 < maze->MazeHeight && maze->plot[i/2][j/2].north) ||
						(i/2-1 > 0 && maze->plot[i/2-1][j/2].south))
						printf("—");
					else
						printf(" ");
			} else{																				//i%2==1 means a | |x| | row
				//test to see if we need an x. If not, do a space
				if(j%2 == 1){
					if (maze->plot[i/2][j/2].has_avatar)
						printf("x");
					else
						printf(" ");
				} else
					//test to see if we need a |. If not, do a space
					if ((j/2 < maze->MazeWidth && maze->plot[i/2][j/2].west) ||
						(j/2-1 > 0 && maze->plot[i/2][j/2-1].east))
						printf("|");
					else
						printf(" ");
			}
		}
		printf("\n");
	}
}


/* *****************************************************************************
* Name: clear_surface
*
* Description: create a blank cairo surface. Borrowed from GTK tutorial
*		example-3.c and modified to fit amazing.c
*
* Input: (void)
*
* Return: (void)
*
*
*/
static void clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}


/* *****************************************************************************
* Name: configure_event_cb
*
* Description: Create a new surface of the appropriate size to display the maze.
*		Borrowed from GTK tutorial example-3.c and modified to fit amazing.c
*
* Input: GtkWidget *widget, pointer to the graphics screen widget
*				 GdkEventConfigure *event, configure event to trigger new surface
*		 		  gpointer data, a pointer containing user data. Unused in this context
*
* Return: gboolean, TRUE
*
*
*/
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event,
	gpointer data)
{
	// If there is a surface, get rid of it
  if (surface)
    cairo_surface_destroy (surface);

	// Define a new surface
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
		CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget),
		gtk_widget_get_allocated_height (widget));

  // Initialize the surface to white
  clear_surface ();

  // We've handled the configure event, no need for further processing.
  return TRUE;
}


/* *****************************************************************************
* Name: draw_cb
*
* Description: Redraw the screen from the surface. Note that the ::draw signal
*		receives a ready-to-be-used cairo_t that is already clipped to only draw the
*		exposed areas of the widget. Borrowed from GTK tutorial example-3.c and
*		modified to fit amazing.c.
*
* Input: GtkWidget *widget, pointer to the graphics screen widget
*				 cairo_t *cr, surface to draw
*		 		 gpointer data, unused
*
* Return: gboolean, FALSE
*
*
*/
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}


/* *****************************************************************************
* Name: draw_maze
*
* Description: draw the maze on the surface
*
* Input: GtkWidget *widget, pointer to the graphics screen widget
*		 		 gpointer data, data containing maze information
*
* Return: (void)
*
*
*/
static void draw_maze (GtkWidget *widget, gpointer data)
{
  cairo_t *cr;

  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

	maze_t *maze = (maze_t *)data;
	guint width, height;
	guint inc, xpos, ypos;

	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);

  	//set increments
	if(width/maze->MazeWidth < height/maze->MazeHeight){
  		inc = width/(maze->MazeWidth+1);
	} else{
		inc = height/(maze->MazeHeight+1);
	}

	//set appropriate line sizes
	if(maze->MazeWidth < 30 && maze->MazeHeight < 30)
  		cairo_set_line_width(cr, 3.0);
  	else if (maze->MazeWidth < 60 && maze->MazeHeight < 60)
  		cairo_set_line_width(cr, 2.0);
  	else
  		cairo_set_line_width(cr, 1.0);

	//create the grid
	for(int i = 0; i< maze->MazeWidth+1; i++){
		xpos = inc/2 + inc*i;
		for(int j = 0; j< maze->MazeHeight+1; j++){
			ypos = inc/2 + inc*j;
			cairo_rectangle (cr, xpos-1,ypos-1, 2, 2);
  			cairo_fill (cr);
		}
	}

  	//draw some maze lines
	for(int i = 0; i<maze->MazeHeight+1; i++){
		for(int j = 0; j<maze->MazeWidth+1; j++){

			//draw north/south walls
			if((i<maze->MazeHeight && j<maze->MazeWidth && maze->plot[i][j].north) ||
				(j<maze->MazeWidth && i>0 && maze->plot[i-1][j].south))

				cairo_move_to(cr, inc/2 + inc*j, inc/2 + inc*i);
				cairo_line_to(cr,inc/2 + inc*(j+1), inc/2 + inc*i);
				cairo_stroke(cr);

			//draw east/west walls
			if((i<maze->MazeHeight && j<maze->MazeWidth && maze->plot[i][j].west) ||
				(i<maze->MazeHeight && j>0 && maze->plot[i][j-1].east)){

				cairo_move_to(cr, inc/2 + inc*j, inc/2 + inc*i);
				cairo_line_to(cr, inc/2 + inc*j, inc/2 + inc*(i+1));
				cairo_stroke(cr);
			}

			//draw avatars
			if(i<maze->MazeHeight && j<maze->MazeWidth && maze->plot[i][j].has_avatar){
				//change the color to red
				cairo_set_source_rgb(cr, 1, 0, 0);

				//draw a rectangle 1/2 the cell dimensions centered in the cell
				cairo_rectangle(cr, inc/2 + inc*j + inc/4, inc/2 + inc*i + inc/4, inc/2,
					inc/2);
				cairo_fill(cr);

				//change the color back
				cairo_set_source_rgb(cr, 0, 0, 0);
			}
		}
	}

  cairo_destroy (cr);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw(widget);
}


/* *****************************************************************************
* Name: draw_maze
*
* Description: function to execute each time the timer ticks in this case, it
*		redraws the maze.
*
* Input: void *data, data to pass to draw_maze (a maze_t data structure)
*
* Return: int, 1
*
*
*/
static int timer(void* data)
{
	clear_surface();
	draw_maze (drawing_area, data);
    return 1;
}


/* *****************************************************************************
* Name: close_window
*
* Description: function to close XQuartz window upon completion. Borrowed from
*	GTK tutorial example-3.c and modified to fit amazing.c
*
* Input: (void)
*
* Return: (void)
*
*
*/
static void close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);

  gtk_main_quit ();
}


/* *****************************************************************************
* Name: activate
*
* Description: function to connect our GtkApplication to our GtkWindow
*
* Input: GtkApplication *app, GtkApplication to add on to
*		 		 gpointer user_data, user_data to pass to later functions
*
* Return: (void)
*
*
*/
static void activate (GtkApplication *app, gpointer user_data)
{

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 8);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (window), frame);


	//set the window dimensions proportional to MazeWidth & MazeHeight
	int wind_width = 100 * ((maze_t *)user_data)->MazeWidth;
	int wind_height = 100 * ((maze_t *)user_data)->MazeHeight;

	//make sure we don't get a window bigger than 1000 pixels in either direction
	while(wind_width > 900 || wind_height > 900){
  		wind_width /= 2;
  		wind_height /=2;
  	}

  drawing_area = gtk_drawing_area_new ();
  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, wind_width, wind_height);

  gtk_container_add (GTK_CONTAINER (frame), drawing_area);

  /* Signals used to handle the backing surface */
  g_signal_connect (drawing_area, "draw",
                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event",
                    G_CALLBACK (configure_event_cb), NULL);

	g_timeout_add(10, timer, user_data);

  gtk_widget_show_all (window);
}


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
void *gui_print_maze(void *maze)
{

	GtkApplication *app;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  	g_signal_connect (app, "activate", G_CALLBACK (activate), maze);
  	g_application_run (G_APPLICATION (app), 0, NULL);
  	g_object_unref (app);

  	return NULL;
}


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
void logMove(FILE *file, int current_avatar, int move_sent, XYPos position){

		char move[10];
		if (move_sent == 0)
			strcpy(move, "West");
		else if (move_sent == 1)
			strcpy(move, "North");
		else if (move_sent == 2)
			strcpy(move, "South");
		else if (move_sent == 3)
			strcpy(move, "East");
		else if (move_sent == 8)
			strcpy(move, "Null move");

		fprintf(file, "\n%s: %d %s: (%d, %d)\t %s: %s\n", "Avatar ID", current_avatar, "	Position",\
			ntohl(position.x), ntohl(position.y), "	Move attempted", move);

}


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
* Return: int, 0 for no error, nonzero if error
*
*
*/
int message_error(AM_Message message, FILE *file, int socket){

	//for each error message returned, close socket and close file

	if (IS_AM_ERROR(message.type)) {
		perror("IS_AM_ERROR message received");
		fprintf(file, "\n%s\n", "IS_AM_ERROR message receivied.");

		if(close(socket) < 0){		//close socket
 			perror("closing stream socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 1;
	}

	//no such avatar error
	else if (ntohl(message.type) == AM_NO_SUCH_AVATAR ) {
		perror("AM_NO_SUCH_AVATAR message received");
		fprintf(file, "\n%s\n", "AM_NO_SUCH_AVATAR message received");

		if(close(socket) < 0){		//close socket
 			perror("closing stream socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 2;
	}

	//too many moves, server will throw error
	else if (ntohl(message.type) == AM_TOO_MANY_MOVES ) {
		perror("AM_TOO_MANY_MOVES message received");
		fprintf(file, "\n%s\n", "AM_TOO_MANY_MOVES message received");

		if(close(socket) < 0){		//close socket
 			perror("closing stream socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 3;
	}

	//unknown message type error
	else if (ntohl(message.type) == AM_UNKNOWN_MSG_TYPE){
		perror("AM_UNKNOWN_MSG_TYPE message received");
		fprintf(file, "\n%s\n", "AM_UNKNOWN_MSG_TYPE message received");

		if(close(socket) < 0){		//close socket
 			perror("closing stream socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 4;
	}

	//unexpected message type error
	else if (ntohl(message.type) == AM_UNEXPECTED_MSG_TYPE) {
		perror("AM_UNEXPECTED_MSG_TYPE message received");
		fprintf(file, "\n%s\n", "AM_UNEXPECTED_MSG_TYPE message received");

		if(close(socket) < 0){				//close socket
 			perror("closing stream socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 5;
	}

	//avatar out of turn error
	else if (ntohl(message.type) == AM_AVATAR_OUT_OF_TURN) {
		perror("AM_AVATAR_OUT_OF_TURN message received");
		fprintf(file, "\n%s\n", "AM_AVATAR_OUT_OF_TURN message received");

		if(close(socket) < 0){ 		//close socket
 			perror("closing socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 6;
	}

	//error in server disk quota being surpassed
	else if (ntohl(message.type) == AM_SERVER_DISK_QUOTA) {
		perror("AM_SERVER_DISK_QUOTA message received");
		fprintf(file, "\n%s\n", "AM_SERVER_DISK_QUOTA message received");

		if(close(socket) < 0){			//close socket
 			perror("closing socket");
 		} else fprintf(file, "Connection closed.\n");
		return 7;

	}

	//error in server out of memory
	else if (ntohl(message.type) == AM_SERVER_OUT_OF_MEM){
		perror("AM_SERVER_OUT_OF_MEM message received");
		fprintf(file, "\n%s\n", "AM_SERVER_OUT_OF_MEM message received");

		if(close(socket) < 0){		//close socket
 			perror("closing socket");
 		} else fprintf(file, "Connection closed.\n");
 		return 8;

	}

	return 0;
}


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
void make_move(int current_avatar, int moveToSend, int socket, FILE *fp){
	//mesage of type avatar_move
	AM_Message message = {.type = htonl(AM_AVATAR_MOVE), .avatar_move.AvatarId = htonl(current_avatar), .avatar_move.Direction = htonl(moveToSend)};


	//send the message to the server
	if (write(socket, &message, sizeof(AM_Message)) < 0){
		perror("sending avatar_move message!");
	}
}

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
int getMove(XYPos avatarZeroPos, XYPos prevPos, XYPos currPos, int prevMove, int avatarID) {

  // Get current position
  int currX = currPos.x;
  int currY = currPos.y;


  // Check if we have found avatar 0
  if (avatarZeroPos.x == currPos.x && avatarZeroPos.y == currPos.y) {
    return M_NULL_MOVE;
  }

  // First move
  if (prevMove == M_NULL_MOVE) {
    return M_NORTH;
  }

  // If we moved last time, try going left
  if (prevPos.x != currX || prevPos.y != currY) {
    // Turn left
    if (prevMove == M_NORTH) {
      return M_WEST;
    }
    if (prevMove == M_EAST) {
      return M_NORTH;
    }
    if (prevMove == M_SOUTH) {
      return M_EAST;
    }
    if (prevMove == M_WEST) {
      return M_SOUTH;
    }
  }
  else {
    // If we didn't move last time, try right
    if (prevMove == M_SOUTH) {
      return M_WEST;
    }
    if (prevMove == M_WEST) {
      return M_NORTH;
    }
    if (prevMove == M_NORTH) {
      return M_EAST;
    }
    if (prevMove == M_EAST) {
      return M_SOUTH;
    }
  }

  // We got stuck. That's not good
  fprintf(stderr, "Avatar %d got stuck. Uh Oh.\n", avatarID);
  return 9;

}
