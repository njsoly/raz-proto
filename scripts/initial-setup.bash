#!/bin/bash
################################################################################
# Make sure your pi is set up with what it needs to run GPIO and whatever else.
################################################################################

# add user to gpio group.
if [ -n "$(groups $USER | grep gpio)" ]; then
	echo "in dialout.";
else
	sudo usermod -a -G gpio $USER
fi

# add user to the dialout group.  I think Arduino IDE needed it to use Rx/Tx, so Pi might, too
if [ -n "$(groups $USER | grep dialout)" ]; then
	echo "in dialout.";
else
	sudo adduser $USER dialout
fi

