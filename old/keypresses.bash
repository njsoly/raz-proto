#!/bin/bash
# this script is a tester.
# it is for messing around with accepting individual, single-keystroke commands,
# eventually to remotely control the Raspberry Pi line follower through SSH 
# in a command-line window.

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
