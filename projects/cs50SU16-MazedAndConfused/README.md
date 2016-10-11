README.md 
The Amazing Challenge

Team MazedAndConfused
Team members: Andrew Werchniak, Robert Sayegh, Cristina Curcelli

CS50 Summer 2016

.c Files: AMStartup.c, amazing_client.c, amazing.c
Header Files: amazing.h (provided by the instructor, but modified) 
	and amazing_client.h

Functionality: 

	Working in conjunction with a server on flume.cs.darmouth.edu,
	AMStartup "solves" mazes sent by the server with a specified difficulty 
	level and number of avatars, with each avatar making moves until the 
	avatars have all "found" on another, i.e. are all at the same point in the 
	maze. 

Compiling: Make 

Usage: 
	
	When connecting to flume, user must include -Y tag with ssh as follows: 
		ssh -Y <user>@flume.cs.dartmouth.edu

	Before running AMStartup, type the following into the terminal while logged 
	into flume: 
		export NO_AT_BRIDGE=1

	To run the AMStartup program, use the following command line: 
		./AMStartup -n [number of Avatars] -d [level of difficulty] -h [hostname]
	Example: 
		./AMStartup -n 5 -d 3 -h flume.cs.dartmouth.edu

	In the terminal, there may be the following messages printed in the 
	terminal as a result of GTK (certain messages may or may not appear depending on machine): 

		libGL error: No matching fbConfigs or visuals found
		libGL error: failed to load driver: swrast *
		
		Gtk-CRITICAL **: gtk_main_quit: assertion 'main_loops != NULL' failed **

		Gtk-CRITICAL **: gtk_widget_get_allocated_width: assertion 'GTK_IS_WIDGET (widget)' failed

		Gtk-CRITICAL **: gtk_widget_get_allocated_height: assertion 'GTK_IS_WIDGET (widget)' failed

		Gtk-CRITICAL **: gtk_widget_queue_draw: assertion 'GTK_IS_WIDGET (widget)' failed

	This messages are due to GTK and are to be ignored. 

	If the GTK drawing area appears black, this indicates that the refresh rate
	is too fast. This should not occur when running the program from the 
	command line but may occur when using valgrind, since it slows the 
	program down. 
	
	* 	these two errors should only occur if XQuartz is being newly launched
	**  This error could have been avoided had we omitted the "pthread_join" call on the graphics
		thread. However, we made the design choice to keep the thread joined, because this 
		prevents the graphics window from quitting immediately when the maze is solved and
		allows the user to really examine the final state of the maze.

Output: There are three distinct outputs for each run of AMStartup (for each
solved maze), as follows: 

	A log file, with name of format Amazing_$USER_N_D.log. The first line of 
	the file is:  $USER, MazePort, and date/time, and the rest of the file logs 
	each avatar's position and attempted move on its turn. 

	ASCII graphics in the terminal's output, where the entire maze is printed 
	once after each avatar takes its turn.

	A GUI written in GTK that shows the avatars moving through the maze, as 
	prompted by a mouse click. 
	
Assumptions: 

	The user will input parameters to the command line as specified in "Usage."
	We assume the server is working to specifications detailed on CS 50 website. 

Limitations: 
	
	GTK by nature produces memory leakes. The instructor and many online 
	sources agreed that these memory leaks are unsolvable. All memory 
	leaks are a result of the use of GTK and not the rest of the code. 

	You must quit the GUI in order for the program to finish running. You also 
	must quit the GUI for the log file to be completed. 

	Higher levels of difficuty require a lot of time to finish solving the maze, particularly 6-8. 

	Our program cannot consistently solve level 9 difficulty. The program exits with a message that the AM_TOO_MANY_MOVES message was received. 

	The maze can be a max height and width of 100 x 100.

	There can only be between 2 and 10 avatars in the maze. 


