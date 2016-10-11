#!/bin/bash

#account for the empty string input case
if [ $# -gt 1 ] ; then
	artist=$1	#first argument is the artist
	shift		#first stored, so no longer need it
else
	artist=""
fi

echo "Files containing Artist tag '$artist':"

#check if there are enough arguments (the user needs to input at least 2)
if [ $# -lt 1 ] ; then		
	echo "Not enough arguments!"
	echo "Please input an artist name and at least one file."
	exit 1
	kill
fi

#for remaining arguments (file names), let's check!
for arg ; do
	#if file doesn't exist, say so and quit
	if ! [ -e "$arg" ] ; then
		echo "Warning! File $arg  does not exist." 
		exit 1 
		kill
	fi

	#if file exists but is of the wrong format, say so and quit
	if exif -m -t 0x013b "$arg" 2>&1 | grep -q "is not readable or does not contain EXIF data!" ; then
        	echo "$arg is not readable or does not contain EXIF data!"
                exit 1
                kill
        fi

	#otherwise, check the file and print it if it contains the artist substring
	if exif -m -t 0x013b "$arg" 2>/dev/null | grep -q "$artist" ; then
		echo "$arg"
	fi
	
done
