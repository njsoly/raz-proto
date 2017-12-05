#!/bin/bash
# can bash do the loop better?



y=; 


printf "type characters to send commands.\n\ttype 'q' to quit.\n"; 
while [ "$x" != "q" ]; do 
	read -s -n1 x; 
	y=$y$x; 
	printf "\r\t'$x' received.     ";
done; 
printf "\nbye. commands: \"${y:0:-1}\"\n"
y=;
x=;
