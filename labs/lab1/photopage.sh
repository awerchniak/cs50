#!/bin/bash

#make sure to test that the parameters are of the right format
if [ $# -ne 3 ]; then
	echo "Error! Incorrect number of arguments"
	echo "Please use the following syntax:"
	echo "photopage.sh newDirectoryName photoDirectoryName templateDirectoryName"
	exit 1
	kill
fi

#if mkdir -p fails, kill the program
if ! mkdir -p "$1" ; then
	echo "Error! mkdir -p failed. Please check the directory you entered"
	exit 1
	kill
fi

#check if photoDirectoryName exists
if ! [ -d $2 ] ; then
	echo "Error! The directory $2 does not exist"
	exit 1
	kill
fi

#check is photoDirectoryName contains any .jpgs
temp=$(ls -1 "$2"/*.jpg | wc -l)
if [ $temp -lt 1 ] ; then
        echo "Error! The directory $2 does not contain any .jpg's!"
        exit 1
        kill
fi

#check if photoDirectoryName contains header.txt
if ! [ -f "$2"/header.txt ] ; then
        echo "Error! The directory $2 does not contain the file header.txt"
	exit 1
	kill
fi

#check if photoDirectoryName contains title.txt
if ! [ -f "$2"/title.txt ] ; then
        echo "Error! The directory $2 does not contain the file title.txt"
        exit 1
        kill
fi

#check if templateDirectoryName exists
if ! [ -d $3 ] ; then
        echo "Error! The directory $3 does not exist"
        exit 1
        kill
fi

#check if templateDirectoryName is missing any of its files
if ! [ -f "$3"/empty.template ] || ! [ -f "$3"/header.template ] || ! [ -f "$3"/index.template ] || ! [ -f "$3"/image.template ] || ! [ -f "$3"/navigation.bottom.template ] || ! [ -f "$3"/navigation.item.template ] || ! [ -f "$3"/navigation.top.template ] ; then
	echo "Error! The directory $3 does not contain all of the templates."
	exit 1
	kill
fi

#create empty.html
cat "$3"/empty.template > "$1"/empty.html

#create header.html
tit=$(cat "$2"/title.txt | sed -e s+\\n+\ +g)
head=$(cat "$2"/header.txt | sed -e s+\\n+\ +g)
cat "$3"/header.template | sed -e "s/@TITLE@/$tit/g" -e "s/@HEADCAPTION@/$head/g" > "$1"/header.html

#create index.html
cat "$3"/index.template | sed -e "s/@TITLE@/$tit/g" > "$1"/index.html

#create image copies, thumbnails, images.html, and navigation.html
mkdir "$1"/images	#make the two directories
mkdir "$1"/thumbs

cat "$3"/navigation.top.template > "$1"/navigation.html

for i in "$2"/*.jpg	#iterate through all jpgs in photo source direc
do
imfil=$(ls $i | sed "s+$2+\/images+g" 2>/dev/null)	#set @@ variables
capt=$(exif -m -t 0x10e "$i" 2>/dev/null || echo "unknown")
date=$(exif -m -t 0x9003 "$i" 2>/dev/null || echo "unknown")
cred=$(exif -m -t 0x13b "$i" 2>/dev/null || echo "unknown")
cpyrt=$(exif -m -t 0x8298 "$i" 2>/dev/null || echo "unknown")
impg=$(ls "$i" | sed -e "s+$2\/++g" -e "s+\.jpg+\.html+g" 2>/dev/null)
thumb=$(ls $i | sed -e "s+$2+\/thumbs+g" 2>/dev/null)

convert "$i" -thumbnail 100x100 "$1""$thumb"	#create thumbnails

scp -p "$i" "$1""$imfil"			#copy photos

cat "$3"/image.template | sed -e "s+@IMAGEFILE@+\.$imfil+g" -e "s+@IMAGEPAGE@+$impg+g" -e "s+@THUMB@+\.$thumb+g" -e "s+@CAPTION@+$capt+g" -e "s+@DATE@+$date+g" -e "s+@CREDIT@+$cred+g" -e "s+@COPYRIGHT@+$cpyrt+g" -e "s+@TITLE@+$tit+g" -e "s+@HEADCAPTION@+$head+g" > "$1"/"$impg" 2>/dev/null		#create image.html

cat "$3"/navigation.item.template | sed -e "s+@IMAGEFILE@+\.$imfil+g" -e "s+@IMAGEPAGE@+$impg+g" -e "s+@THUMB@+\.$thumb+g" -e "s+@CAPTION@+$capt+g" -e "s+@DATE@+$date+g" -e "s+@CREDIT@+$cred+g" -e "s+@COPYRIGHT@+$cpyrt+g" -e "s+@TITLE@+$tit+g" -e "s+@HEADCAPTION@+$head+g" >> "$1"/navigation.html 2>/dev/null	
							#append contents

done
cat "$3"/navigation.bottom.template >> "$1"/navigation.html	#add bottom

