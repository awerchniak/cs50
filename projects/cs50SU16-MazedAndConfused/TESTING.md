TESTING.md 
The Amazing Challenge

Team Mazed and Confused
Andrew Werchniak, Robert Sayegh, and Cristina Curcelli 

CS50 Summer 2016 

Edge cases: we simulated all of the server return messages and evaluated the output with valgrind to ensure proper responses are being made when the avatars receive an error message from the server. 

Difficulty levels 0-4: We've run the lower levels of difficulty with a variety of numbers of avatars in the maze. We verified output, particularly the "solved" position, with the images of the lower level mazes provided on the CS 50 website and accessed via /var/tmp/<Mazeport> 

Difficulty levels 5-8: We ran various different numbers of avatars on the medium difficulty levels without problems. 

Difficulty level 9: Our program has not solved a maze of difficulty level 9. Eventually the GUI stops moving, we can see in the terminal that moves are no longer being made, and upon exiting the GUI we see the AM_TOO_MANY_MOVES message, as we should. 

